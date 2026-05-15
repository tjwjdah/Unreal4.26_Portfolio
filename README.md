[README_readable.md](https://github.com/user-attachments/files/27806250/README_readable.md)
# Survival Game - Unreal Engine C++ 클라이언트 포트폴리오

> Unreal Engine C++ 기반 3D 생존/액션 게임 클라이언트 프로젝트입니다.  
> 대량으로 배치되는 건축물과 자원을 **HISM 기반 청크 시스템**으로 관리하고,  
> **ID / InstanceIndex 매핑**, **청크 스트리밍**, **Dirty Chunk 저장/로드**, **DataTable 기반 콘텐츠 관리**, **AI 전투 시스템**을 구현했습니다.

---

<a id="table-of-contents"></a>
## 목차

- [1. 프로젝트 개요](#overview)
- [2. 핵심 기술 요약](#tech-summary)
- [3. 주요 기능 요약](#feature-summary)
- [4. 시스템 구조](#system-architecture)
- [5. 핵심 기술 상세](#core-tech)
  - [5.1 HISM 기반 청크 월드 관리](#hism-chunk-world)
  - [5.2 플레이어 위치 기반 청크 스트리밍](#chunk-streaming)
  - [5.3 Dirty Chunk 기반 저장/로드](#dirty-chunk-save-load)
  - [5.4 소켓 기반 건축 연결과 HISM ↔ Actor Hybrid](#socket-building-state)
  - [5.5 데이터 기반 건축 시스템과 에셋 로드](#data-driven-building)
  - [5.6 인벤토리 / 제작 / 퀘스트 UI](#inventory-crafting-quest)
  - [5.7 AI 전투 시스템](#ai-combat)
- [6. 코드 구조](#code-structure)
- [7. 측정 방법 / 재현 조건](#measurement-method)
- [8. 설계상 트레이드오프](#tradeoffs)
- [9. 한 줄 요약](#one-line-summary)

---

<a id="overview"></a>
## 1. 프로젝트 개요

| 항목 | 내용 |
|---|---|
| 프로젝트명 | Survival Game |
| 장르 | 3D 생존 / 액션 / 제작 / 건축 |
| 엔진 | Unreal Engine C++ 4.26 |
| 담당 영역 | 클라이언트 시스템 설계 및 구현 |
| 주요 구현 | 청크 스트리밍, HISM 인스턴싱, 건축 시스템, 저장/로드, 인벤토리, 제작, 퀘스트, AI 전투 |
| 개발 인원 | 1명 |
| 개발 기간 | 3~4개월 |

### 핵심 목표

- 건축물과 자원을 개별 Actor가 아닌 **HISM 인스턴스**로 관리합니다.
- HISM이 자체 상태를 가질 수 없는 문제는 **BuildingID / ResourceID 기반 상태 관리**합니다.
- 플레이어 주변 청크만 로드하고, 멀어진 청크는 SaveData 캐시에 반영한 뒤 언로드합니다.
- 건축물 배치, 소켓 연결, 내구도, 업그레이드, 파괴, 저장/로드를 하나의 흐름으로 묶습니다
- 건축물, 아이템, 퀘스트 정보는 DataTable로 분리해 확장성을 높힙니다.

---

<a id="tech-summary"></a>
## 2. 핵심 기술 요약

| 핵심 기술 | 설명 |
|---|---|
| HISM 기반 오브젝트 관리 | 건축물과 자원을 HISM 인스턴스로 등록해 Actor / Component 수 증가를 줄였습니다. |
| ID ↔ InstanceIndex 매핑 | HISM 인스턴스를 개별 오브젝트처럼 다루기 위해 `BuildingID` / `ResourceID`와 Hit된 HISM Component의 `InstanceIndex`를 매핑했습니다. |
| Chunk Streaming | 플레이어 위치 기준으로 필요한 청크만 로드하고, 범위 밖 청크는 저장 캐시에 반영 후 언로드합니다. |
| Connected Chunk Loading | 청크 밖에 연결된 건축물이 누락되지 않도록 `SocketMap`을 따라 필요한 청크를 추가 로드합니다. |
| Dirty Chunk Save | 전체 월드를 매번 저장하지 않고, 변경된 청크만 SaveData에 반영합니다. |
| DataTable + Soft Reference | 건축물, 아이템, 퀘스트 정보를 DataTable로 분리하고, 건축 프리뷰 에셋은 비동기로 로드해 캐싱합니다. |
| HISM ↔ Actor Hybrid | 정적 건축물은 HISM으로 줄이고, 문처럼 상호작용이 필요한 오브젝트는 Actor로 유지했습니다. |
| Behavior Tree 기반 AI | Task / Service / Decorator로 몬스터 행동과 보스 Phase 패턴을 분리했습니다. |

### 핵심 설계 흐름

```mermaid
flowchart LR
    A[대량 건축물 / 자원 오브젝트] --> B[HISM Instance 등록]
    B --> C[ObjectID / InstanceIndex 매핑]
    C --> D[LineTrace 결과로 ID 역조회]
    D --> E[내구도 / 소켓 / 파괴 / 채집 / 저장 처리]
    C --> F[플레이어 주변 청크만 로드]
    F --> G[변경된 청크만 Dirty Save]
    F --> H[연결된 건축물 청크 추가 로드]
```

단순히 HISM을 사용한게 아니라 **HISM 인스턴스에 ID와 상태 데이터를 연결해서, 개별 건축물/자원처럼 조작할 수 있게** 만들었습니다.

청크 스트리밍은 렌더링 컬링과 다르게 멀어진 **청크의 HISM 인스턴스와 충돌 데이터를 실제 월드에서 제거**하고, SaveData 캐시만 남기는 방식으로 동작합니다.

---

<a id="feature-summary"></a>
## 3. 주요 기능 요약

| 구분 | 주요 구현 내용 |
|---|---|
| 월드 / 청크 시스템 | 플레이어 주변 청크를 로드/언로드하고, HISM Hit 결과를 `BuildingID` / `ResourceID`로 역조회해 채집, 파괴, 업그레이드, 저장/로드에 사용했습니다. |
| 건축 시스템 | 건축 모드, 프리뷰, 충돌 검사, 소켓 스냅, 배치, 내구도, 파괴, 업그레이드, 문 상호작용을 구현했습니다. |
| 저장 / 로드 | 청크별 건축물, 자원, 부착형 Actor, 소켓 연결 정보를 저장하고 다시 복원합니다. |
| 인벤토리 / 제작 / 퀘스트 | UMG 기반 인벤토리와 퀵슬롯, 아이템 스택, 제작 UI, 사냥/제작 등 퀘스트 진행도와 보상 지급을 구현했습니다.|
| 전투 / AI | 장비 타입별 공격, 활 조준/발사, 스태미나 소모, Behavior Tree 기반 몬스터 AI, 보스 Phase 패턴을 구현했습니다. |

---

<a id="system-architecture"></a>
## 4. 시스템 구조

```mermaid
flowchart TD
    Player[APlayerCharacter] --> BuildManager[UBuildManagerComponent]
    BuildManager --> BuildAssetSubsystem[UBuildAssetSubsystem]
    BuildAssetSubsystem --> BuildDataTable[FBuildDefinitionInfo DataTable]
    BuildManager --> Preview[Build Preview Actor]
    Preview --> ChunkManager[AChunkManager]

    ChunkManager --> ChunkActor[AChunkActor]
    ChunkActor --> HISM[HISM Components]
    HISM --> IDMap[InstanceIndex ↔ ObjectID Mapping]
    IDMap --> RuntimeMap[BuildingMap / ResourceMap]
    RuntimeMap --> SaveGame[UChunkSaveGame]
    ChunkManager --> SaveGame

    Player --> Inventory[Inventory / Quick Slot UI]
    Inventory --> ItemData[FUIItemTableInfo DataTable]
    Player --> Quest[Quest UI]
    Quest --> QuestData[FQuestTableInfo DataTable]
    Monster[Monster / Morigesh] --> BT[Behavior Tree Tasks]
```

---

<a id="core-tech"></a>
## 5. 핵심 기술 상세

<a id="hism-chunk-world"></a>
### 5.1 HISM 기반 청크 월드 관리

#### 문제

건축물, 나무, 돌 같은 월드 오브젝트가 많아질수록 개별 Actor 방식은 부담이 커집니다.  
특히 생존/건축 게임에서는 플레이어가 만든 건축물이 계속 쌓이고, 자원 오브젝트도 월드에 많이 존재합니다.

단순히 Tick을 끄거나 멀리 있는 오브젝트를 숨기는 것만으로는 부족했습니다.  
그래서 여러 최적화 방식을 찾아봤고, 그중 대량 오브젝트 관리에 가장 적합하다고 판단한 HISM을 기반으로 구조를 바꿨습니다.

#### 왜 HISM을 선택했나?

| 방법 | 장점 | 한계 | 판단 |
|---|---|---|---|
| 개별 Actor 유지 + Tick 비활성화 | 기존 로직을 그대로 쓰기 쉬움 | Actor / Component / Collision 비용은 계속 남음 | 대량 오브젝트에는 부족 |
| 거리 기반 숨김 / Collision 비활성화 | 멀리 있는 오브젝트 비용 일부 감소 | Actor는 계속 월드에 남음 | 보조 최적화로만 적합 |
| Object Pooling | Spawn / Destroy 비용 감소 | 이미 배치된 오브젝트는 계속 Actor로 유지해야 함 | 생성 비용 완화용 |
| Merge Actor / HLOD | 고정 배경 오브젝트에 유리 | 자유 배치, 파괴, 업그레이드가 필요한 건축물에는 맞지 않음 | 부적합 |
| ISM | 같은 Mesh를 인스턴스로 관리 가능 | 공간 단위 관리와 컬링 측면에서 HISM보다 아쉬움 | 후보였지만 HISM이 더 적합 |
| HISM | 반복 Mesh를 인스턴스로 관리하고 계층 컬링에 유리 | Actor가 아니므로 상태 관리를 직접 해야 함 | ID 기반 상태 구조를 추가해 채택 |

결국 HISM을 기반으로 하되, HISM 인스턴스를 개별 게임 오브젝트처럼 다루기 위한 **ID 기반 관리 구조**를 직접 만들었습니다.

#### 구현

`AChunkManager`는 전체 청크와 런타임 상태를 관리하고, `AChunkActor`는 청크 내부 HISM 인스턴스를 관리합니다.

- `AChunkManager`
  - 청크 로드/언로드
  - 건축물/자원 ID 발급
  - `BuildingMap`, `ResourceMap` 관리
  - 저장/로드 및 Dirty Chunk 처리

- `AChunkActor`
  - 청크 내부 HISM Component 생성/재사용
  - HISM `InstanceIndex`와 ObjectID 매핑
  - HISM Hit 결과로 ID 역조회

```cpp
TMap<FChunkCoords, AChunkActor*> LoadChunks;
TMap<int32, FBuildingNode> BuildingMap;
TMap<int32, FResourceNode> ResourceMap;
```

#### ID 기반 상태 관리

HISM 인스턴스는 Actor가 아니기 때문에 아래 정보를 직접 가질 수 없습니다.

- 고유 ID
- 내구도 / 최대 내구도
- 소켓 점유 상태
- 연결된 건축물 목록
- 파괴 / 업그레이드 상태
- 저장/로드 데이터

그래서 건축물과 자원에 각각 고유 ID를 부여하고, 실제 상태는 `BuildingMap`, `ResourceMap`에서 관리했습니다.

```text
BuildingID
 ├─ OwnerChunk
 ├─ InstanceIndex
 ├─ StaticMesh
 ├─ Durability
 ├─ MaxDurability
 ├─ BuildDefinitionName
 ├─ MaterialType / BuildType
 ├─ SocketMap
 ├─ ConnectedBuildingIDs
 └─ IsAnchor

ResourceID
 ├─ OwnerChunk
 ├─ InstanceIndex
 ├─ StaticMesh
 ├─ Hp
 ├─ ResourceType
 └─ ResourceDefinitionName
```

건축물은 소켓 연결, 업그레이드, 파괴, 지지 구조 판정이 필요해서 `SocketMap`, `ConnectedBuildingIDs`, `IsAnchor`를 추가로 관리했습니다.  
자원은 채집/파괴에 필요한 체력, 자원 타입, 정의 이름 위주로 관리했습니다.

```text
LineTrace Hit
    ↓
HISM Component + InstanceIndex
    ↓
ChunkActor에서 ObjectID 역조회
    ↓
BuildingMap / ResourceMap 접근
    ↓
Damage / Upgrade / Destroy / Harvest / Save 처리
```

#### 벤치마크 조건

| 항목 | 값 |
|---|---|
| 배치 수 | `GridSizeX 30 * GridSizeY 30 * GridSizeZ 5 = 4,500개` |
| Mesh | `[측정에 사용한 Mesh 이름]` |
| 간격 | `Spacing 300.f` |
| 측정 방식 | 동일 위치에서 3~5회 평균 |
| 측정 환경 | `[CPU / GPU / RAM / 해상도 / 빌드 타입]` |

#### 측정 결과

| 방식 | 생성 시간(ms) | 평균 FPS | Game Thread(ms) | Render Thread(ms) | GPU(ms) | Draw Calls | Actor 수 | Component 수 | 비고 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| Actor | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 4,500 | [ ] | 개별 Actor Spawn |
| ISM | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 1 | [ ] | 단일 ISM Component |
| HISM | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 1 | [ ] | 단일 HISM Component |
| Chunk-HISM | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | `[청크 수]` | `[HISM 수]` | 청크 + ID/Socket 관리 포함 |

#### 측정 후 넣을 해석 예시

```text
동일한 4,500개 오브젝트 배치 조건에서 Actor, ISM, HISM, Chunk-HISM 방식을 비교했습니다.
Actor 방식은 개별 오브젝트 관리에는 편하지만, 오브젝트 수가 늘어날수록 Actor 수와 Component 수가 그대로 증가합니다.
반면 HISM/Chunk-HISM 방식은 반복 Mesh를 인스턴스 단위로 관리해 더 적은 Component로 대량 오브젝트를 표현할 수 있었습니다.
```

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| HISM 선택 이유 아래 | Actor / ISM / HISM 구조 비교 이미지 |
| ID 기반 상태 관리 아래 | LineTrace → InstanceIndex → ObjectID 역조회 흐름도 |
| 측정 결과 아래 | Actor / ISM / HISM / Chunk-HISM 비교 그래프 |

#### 관련 코드

```text
PP/Chunk/ChunkManager.h
PP/Chunk/ChunkManager.cpp
PP/Chunk/ChunkActor.h
PP/Chunk/ChunkActor.cpp
PP/Tools/AInstancingBenchmark.h
PP/Tools/AInstancingBenchmark.cpp
```

---

<a id="chunk-streaming"></a>
### 5.2 플레이어 위치 기반 청크 스트리밍

#### 문제

청크 시스템은 처음에는 스트리밍용으로 만든 것이 아니었습니다.  
월드 좌표를 `FChunkCoords`로 바꾸고, 그 좌표를 키로 건축물이나 자원을 빠르게 찾기 위한 구조였습니다.

```text
World Location
    ↓
FChunkCoords
    ↓
Chunk Map 조회
    ↓
해당 청크의 오브젝트 접근
```

하지만 이 방식은 포트나이트처럼 정해진 격자에 맞춰 배치하는 시스템에 더 어울렸습니다.  

이 프로젝트는 러스트나 더 포레스트처럼 플레이어가 원하는 위치와 방향으로 자유롭게 배치하는 방식을 목표로 했기 때문에, 청크 좌표만으로 특정 건축물을 식별하기 어려웠습니다.

그래서 상호작용 대상은 `HISM Component + InstanceIndex`로 찾고, 청크 시스템은 **청크 스트리밍**과 **Dirty Chunk 기반 저장/로드**에 활용하는 방향으로 전환했습니다.

#### 설계 전환

```text
초기 목적: 월드 좌표 → 청크 좌표 → 오브젝트 빠른 조회
    ↓
한계 발견: 자유 배치 / 회전 / 소켓 / 경계 걸침 때문에 좌표만으로 식별 어려움
    ↓
역할 전환: 청크를 오브젝트 조회 단위가 아니라 월드 데이터 관리 단위로 사용
    ↓
적용 기능: 청크 스트리밍, Dirty Save
```

#### 구현

플레이어 위치를 기준으로 중심 청크를 계산하고, 반경 안의 청크만 로드했습니다.  
멀어진 청크는 SaveData 캐시에 반영한 뒤 월드에서 제거하고, 다시 가까워지면 SaveData를 기준으로 HISM 인스턴스를 재생성합니다.

```cpp
FChunkCoords CenterCoords = GetChunkCoordsLocation(Location);
TSet<FChunkCoords> WantChunks = BuildWantChunkSet(CenterCoords);
TSet<FChunkCoords> FinalWantChunks = BuildConnectedWantedChunkSet(WantChunks);
```

#### 구현 포인트

- `StreamingRadius` 기준으로 로드 대상 청크를 계산합니다.
- 현재 로드된 청크와 비교해 Load / Unload 목록을 나눕니다.
- 언로드 전 변경된 청크는 SaveData 캐시에 반영합니다.
- 범위 밖 청크의 HISM 인스턴스와 Collision 데이터를 실제 월드에서 제거합니다.
- 청크 경계에 걸친 건축물을 위해 연결된 청크도 추가 로드합니다.

#### 경계 연결 처리

기본 반경만 로드하면 문제가 생길 수 있습니다.  
예를 들어 현재 청크의 천장이 바깥 청크의 Foundation에 연결되어 있다면, 바깥 청크가 언로드되면서 지지 구조나 소켓 연결 판정이 어긋날 수 있습니다.

이를 줄이기 위해 먼저 플레이어 주변 청크를 기본 로드 대상으로 잡은 뒤 Runtime SaveData의 `SocketMap`을 기준으로 연결된 건축물 ID를 BFS로 순회하고, 연결된 건축물이 다른 청크에 있으면 그 청크도 최종 로드 대상에 추가했습니다.

```text
Player Location → CenterCoords 계산
    ↓
BuildWantChunkSet으로 기본 반경 청크 수집
    ↓
기본 청크들을 Queue에 추가
    ↓
Runtime SaveData에서 CurrentChunk의 Building SaveData 조회
    ↓
Building.SocketMap의 연결 BuildingID 확인
    ↓
ChunkBuildingDataIndexCoords에서 연결 BuildingID의 청크 좌표 조회
    ↓
아직 로드 대상이 아니라면 FinalWantedChunks에 추가 후 Queue에 Enqueue
    ↓
FinalWantedChunks 기준으로 Load / Unload 수행
```

이 방식은 연결된 건축물이 여러 청크에 걸쳐 있더라도 연결 그래프를 따라 필요한 청크를 함께 로드할 수 있다는 장점이 있습니다. 대신 하나의 대형 구조물이 많은 청크를 가로지르면 활성 청크 수가 증가할 수 있으므로, 연결 안정성과 메모리 사용량 사이의 트레이드오프가 있습니다.

#### 정량 측정 항목

| 방식 | TotalKnownChunks | ActiveChunks | UnloadedChunks | ActiveTotalInstances | ActiveHISMComponents | AvgLoad(ms) | AvgUnload(ms) | AvgSave(ms) | 비고 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| 전체 로드 | [ ] | [ ] | [ ] | [ ] | [ ] | - | - | - | `bEnableChunkStreaming = false` |
| StreamingRadius = 1 | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 중심 청크 주변만 로드 |
| StreamingRadius = 2 | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 반경 확장 비교 |
| StreamingRadius = 1 + 연결 청크 로드 | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | 경계 연결 안정성 비교 |

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| 설계 전환 아래 | 자유 배치 건축물과 청크 격자가 어긋나는 예시 |
| 구현 아래 | 플레이어 이동에 따라 청크가 로드/언로드되는 GIF |
| 경계 연결 처리 아래 | 기본 로드 청크와 추가 로드 청크를 색으로 구분한 다이어그램 |
| 측정 항목 아래 | 전체 로드 vs 청크 스트리밍 ActiveChunks 비교 그래프 |

#### 관련 코드

```text
AChunkManager::UpdateChunkStreamingAroundLocation
AChunkManager::LoadChunkRuntime
AChunkManager::UnloadChunkRuntime
AChunkManager::BuildWantChunkSet
AChunkManager::BuildConnectedWantedChunkSet
```

---

<a id="dirty-chunk-save-load"></a>
### 5.3 Dirty Chunk 기반 저장/로드

#### 문제

청크 좌표는 자유 배치된 건축물을 직접 찾는 용도로는 한계가 있었지만 저장/로드 단위로는 잘 맞았습니다.

월드 전체를 매번 저장하면 건축물과 자원이 많아질수록 저장 대상이 계속 커지고, 플레이어가 일부 지역만 수정했더라도 전체 데이터를 다시 구성해야 합니다

그래서 청크를 **변경된 월드 데이터를 묶어서 저장하고 복원하는 단위**로 사용했습니다.

#### 구현

건축물 배치, 파괴, 업그레이드, 자원 채집처럼 월드 상태가 바뀌면 해당 위치의 청크만 Dirty로 표시했습니다.  
저장 시에는 Dirty Chunk만 SaveGame 데이터에 반영합니다.

```cpp
TSet<FChunkCoords> DirtyChunks;
TSet<FChunkCoords> GraphDirtyChunks;
```

- `DirtyChunks`: 일반적인 건축물/자원 변경
- `GraphDirtyChunks`: 소켓 연결처럼 주변 구조와 함께 저장해야 하는 변경

스트리밍 중에는 Runtime SaveData 캐시를 유지하고, 청크 언로드 또는 수동 저장 시 변경된 청크만 캐시에 반영했습니다.

#### 저장 데이터 구조

```cpp
USTRUCT(BlueprintType)
struct FBuildingSaveData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    int32 ID = INDEX_NONE;

    UPROPERTY(SaveGame)
    FString BuildDefinitionName;

    UPROPERTY(SaveGame)
    FTransform Transform;

    UPROPERTY(SaveGame)
    int32 Durability = 0;

    UPROPERTY(SaveGame)
    TMap<FName, int32> SocketMap;
};

USTRUCT(BlueprintType)
struct FResourceSaveData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    int32 ID = INDEX_NONE;

    UPROPERTY(SaveGame)
    FTransform Transform;

    UPROPERTY(SaveGame)
    int32 Hp = 0;

    UPROPERTY(SaveGame)
    EnvironmentsType ResourceType = EnvironmentsType::Wood;

    UPROPERTY(SaveGame)
    FString ResourceDefinitionName;
};
```

#### 구현 포인트

- 건축물/자원 변경 시 위치 기반으로 Dirty Chunk 등록
- 연결 관계 변경 시 Graph Dirty Chunk 등록
- `SaveWorldFull`과 `SaveWorldDirty` 분리
- 청크 언로드 전 Dirty 상태를 SaveData 캐시에 반영
- 문처럼 HISM에 직접 포함되지 않는 부착형 Actor는 별도 SaveData로 저장

#### 보완한 부분: 스트리밍과 저장 캐시 동기화

청크를 언로드하기 전에 변경 내용을 캐시에 반영하지 않으면, 다시 로드했을 때 이전 상태로 돌아갈 수 있습니다.  
예를 들어 파괴한 건축물이 다시 생기거나, 업그레이드 전 상태로 복원될 수 있습니다.

그래서 언로드 전 Dirty Chunk를 캐시에 반영하고, 수동 저장 시에도 Dirty Chunk만 SaveGame 데이터에 갱신하도록 분리했습니다.  
또한 연결 구조가 바뀐 경우에는 연결 정보가 포함된 청크도 함께 Dirty 처리했습니다.

#### 정량 측정 항목

| 방식 | 전체 청크 수 | Dirty 청크 수 | 저장 대상 청크 수 | Save Time(ms) | Save Slot Size(KB/MB) | 비고 |
|---|---:|---:|---:|---:|---:|---|
| Full Save | [ ] | - | [ ] | [ ] | [ ] | 전체 청크 저장 |
| Dirty Save - 1개 청크 수정 | [ ] | 1 | 1 | [ ] | [ ] | 일부 청크만 갱신 |
| Dirty Save - 5개 청크 수정 | [ ] | 5 | 5 | [ ] | [ ] | 다중 청크 갱신 |

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| 구현 아래 | 전체 저장과 Dirty Save 차이 다이어그램 |
| 저장 데이터 구조 아래 | BuildingSaveData / ResourceSaveData 관계도 |
| 캐시 동기화 아래 | Unload 전 SaveData 캐시에 반영되는 흐름도 |
| 측정 항목 아래 | Full Save / Dirty Save 저장 시간 비교 그래프 |

#### 관련 코드

```text
PP/Chunk/ChunkSaveGame.h
AChunkManager::SaveWorld
AChunkManager::SaveWorldFull
AChunkManager::SaveWorldDirty
AChunkManager::LoadWorld
AChunkManager::BuildChunkSaveData
AChunkManager::FlushDirtyChunksSaveCache
```

---

<a id="socket-building-state"></a>
### 5.4 소켓 기반 건축 연결과 HISM ↔ Actor Hybrid

#### 문제

건축 시스템은 완전 자유 배치만으로는 부족했습니다.  
벽, 바닥, 천장, 계단, 문틀처럼 서로 맞물려야 하는 건축물은 위치와 회전이 정확해야 하고, 어떤 건축물의 어느 지점에 붙었는지도 계속 관리해야 했습니다.

단순히 충돌 검사만으로 가까운 건축물을 찾는 방식도 애매했는데 연결이 불가능한 건축물이거나, 이미 사용 중인 연결 지점에 다른 건축물이 또 붙는 문제도 생길 수 있기 때문입니다.

그래서 건축물 연결은 **Static Mesh Socket**을 기준으로 처리했습니다.  
소켓은 각 건축물 Mesh에 미리 정의된 연결 지점이기 때문에, 벽은 벽에 맞는 위치로, 천장은 천장에 맞는 위치로, 계단은 계단에 맞는 위치로 자연스럽게 붙일 수 있었습니다.

#### HISM ↔ Actor Hybrid를 같이 사용한 이유

벽, 바닥, 천장, 창문, 계단처럼 배치 후 거의 움직이지 않는 건축물은 HISM으로 관리했습니다.  
많이 배치될 수 있는 오브젝트라 Actor로 하나씩 유지하기엔 부담이 컸기 때문입니다.

하지만 문처럼 열림/닫힘, 애니메이션, 충돌 변경이 필요한 오브젝트는 HISM만으로 처리하기 어려웠습니다.  
그래서 정적 건축물은 HISM으로 줄이고, 문처럼 직접 상호작용이 필요한 오브젝트만 Actor로 유지했습니다.

대신 두 방식 모두 `BuildingID` 기준으로 관리하여 소켓 연결, 내구도, 파괴, 업그레이드, 저장/로드는 같은 흐름으로 처리할 수 있었습니다.

#### 검토한 방식

| 방식 | 장점 | 한계 | 판단 |
|---|---|---|---|
| 모든 건축물을 Actor로 유지 | 문 상호작용, 애니메이션, 충돌 변경 구현이 쉬움 | 벽, 바닥, 천장처럼 대량 배치되는 정적 건축물까지 Actor 수가 증가 | 성능 측면에서 불리 |
| 모든 건축물을 HISM으로 관리 | 대량 건축물 관리에 가장 유리 | 문처럼 상태 변화와 상호작용이 필요한 오브젝트 구현이 복잡 | 기능 구현 비용이 큼 |
| HISM으로 두고 상호작용 시에만 임시 Actor 생성 | 평상시 Actor 수를 최소화할 수 있고, 상호작용 오브젝트가 많아질수록 장기적으로 유리 | HISM ↔ Actor 전환 시 Transform, Collision, Animation, 저장 데이터, 소켓 연결 상태를 모두 동기화해야 함 | 장기 개선 후보 |
| 정적 건축물은 HISM, 상호작용 Actor만 유지 | 현재 프로젝트처럼 상호작용 Actor 개수가 적은 구조에서는 구현이 단순하고 안정적 | 상호작용 Actor가 많아지면 Actor 수와 별도 저장 데이터가 증가 | 현재 프로젝트 범위에서 최종 채택 |

현재 프로젝트에서는 상호작용 오브젝트가 문 밖에 없어, 전체 정적 건축물에 비해 수가 적다고 봤습니다. 그래서 **정적 건축물은 HISM으로 관리하고 문만 Actor로 유지하는 방식**으로 구현했습니다.

다만 보관함, 제작대, 화로, 함정처럼 상태를 가진 상호작용 오브젝트가 많아진다면, 평상시에는 HISM으로 두고 상호작용 시점에만 Actor로 전환하는 구조가 더 적합할 수 있습니다.

#### 구현

정적 건축물은 HISM 인스턴스로 전환하고, 문처럼 상호작용이 필요한 오브젝트는 별도 Actor로 유지했습니다.
이렇게 하면 많이 배치되는 벽, 바닥, 천장, 창문, 계단은 HISM으로 부담을 줄이고, 문은 Actor의 충돌, 함수 호출, Timeline/애니메이션, 상태 저장 구조를 활용할 수 있습니다.

건축물 연결은 Static Mesh Socket을 사용했습니다.
배치 시 가장 가까운 유효 소켓을 찾아 프리뷰 위치와 회전을 보정하고, 배치 완료 후 SocketMap과 ConnectedBuildingIDs를 갱신했습니다.

```text
Build Preview
    ↓
라인트레이스로 연결 대상 건축물 탐색
    ↓
AllowedTargetTypes / AllowedTargetSockets 확인
    ↓
가장 가까운 유효 Socket 찾기
    ↓
Preview Transform을 Socket 기준으로 보정
    ↓
충돌 검사 및 배치 가능 여부 확인
    ↓
배치 확정
    ↓
HISM Instance 등록 또는 Actor 생성
    ↓
SocketMap / ConnectedBuildingIDs 갱신
```

```cpp
bool AChunkManager::AddSocket(
    int32 ID,
    int32 ConnectID,
    FName SocketName,
    FName ConnectSocketName
);

TMap<FName, int32> SocketMap;
TArray<int32> ConnectedBuildingIDs;
```

`SocketMap`은 특정 소켓에 어떤 건축물이 붙어 있는지 관리합니다.
이미 사용 중인 소켓에는 다시 배치할 수 없도록 막았습니다.

`ConnectedBuildingIDs`는 이 건축물이 어떤 건축물과 연결되어 있는지 저장합니다.
이 정보는 배치뿐 아니라 저장/로드, 청크 경계 로드, 파괴 후 지지 구조 판정에도 사용했습니다.

#### 구현 포인트

- `AllowedTargetTypes`, `AllowedTargetSockets`로 배치 가능한 대상 타입과 소켓을 제한했습니다.
- 가장 가까운 유효 소켓을 찾아 프리뷰 위치와 회전을 보정했습니다.
- 이미 사용 중인 소켓은 `SocketMap`으로 막아 중복 배치를 방지했습니다.
- 배치 완료 시 양쪽 건축물의 연결 정보를 서로 등록했습니다.
- 정적 건축물은 배치 후 HISM 인스턴스로 전환했습니다.
- 문 Actor는 부모 건축물 ID와 소켓 이름을 기준으로 부착 관리했습니다.
- 소켓 연결 정보는 저장/로드, 청크 경계 연결, 파괴 후 지지 구조 판정에도 사용했습니다.

#### 구현 중 보완한 부분: 충돌 검사 기반 판정에서 연결 그래프 기반 지지 판정으로 변경

처음에는 건축물이 파괴될 때 주변 충돌 검사를 통해 인접한 건축물을 찾았습니다.  
하지만 여러 건축물이 한 번에 무너지는 상황에서는 각 건축물마다 충돌 검사가 반복되어 비용이 커졌습니다.

또 충돌 검사만으로는 “가까이 있는 건축물”과 “실제로 소켓으로 연결된 건축물”을 구분하기 어려웠습니다.
서로 연결되지 않았는데 가까이 있다는 이유로 연결된 것처럼 판정될 수 있었고, 어떤 건축물이 지지 구조인지 판단하는 코드도 복잡해졌습니다.

그래서 배치 시점에 저장해 둔 `ConnectedBuildingIDs`를 연결 그래프처럼 사용했습니다.  
건축물을 파괴할 때는 파괴 대상과 연결되어 있던 건축물들을 시작점으로 BFS를 돌고, 탐색된 연결 컴포넌트 안에 `IsAnchor`가 있는지 확인했습니다.

```text
DestroyBuilding(ID)
    ↓
파괴 대상의 ConnectedBuildingIDs를 StartNodes로 저장
    ↓
파괴 대상을 SocketMap / ConnectedBuildingIDs에서 Disconnect
    ↓
파괴 대상 HISM Instance 제거
    ↓
각 StartNode에서 ConnectedBuildingIDs 기반 BFS 탐색
    ↓
연결 컴포넌트 내부에 IsAnchor가 있는지 확인
    ↓
Anchor가 없으면 해당 연결 컴포넌트의 건축물들을 함께 제거
```

이 방식은 매번 주변 충돌 검사를 반복하지 않고, 실제 배치 과정에서 확정된 연결 정보만 사용합니다.
그래서 가까이 있지만 연결되지 않은 건축물이 잘못 포함되는 문제를 줄이고, 대량 파괴 상황도 연결 그래프 단위로 처리할 수 있습니다.

#### 선택적으로 넣을 수 있는 정량 지표

이 섹션은 성능 수치보다 기능 안정성 지표와 GIF가 더 효과적입니다.

| 항목 | 값 | 설명 |
|---|---:|---|
| 지원 건축물 타입 수 | [ ] | Foundation, Wall, Ceiling, DoorFrame, Door, Window, Stairs 등 |
| Socket 기반 연결 테스트 수 | [ ] | 수동 테스트 또는 자동 테스트 기준 |
| 중복 배치 방지 케이스 수 | [ ] | 이미 사용 중인 SocketMap 검증 케이스 |
| 업그레이드 단계 수 | [ ] | Wood → Stone → Metal 등 |
| 내구도 단계 | [ ] | 재질별 Durability 값 |
| 연결 파괴 테스트 수 | [ ] | Anchor 유무에 따른 연결 구조 파괴 검증 |

#### 추천 이미지/GIF 위치

| 위치 | 추천 자료 |
|---|---|
| `왜 소켓 기반으로 연결했는가?` 아래 | 자유 배치와 소켓 스냅의 차이를 보여주는 다이어그램 |
| `소켓 기반 배치 흐름` 아래 | 프리뷰가 가장 가까운 소켓에 스냅되는 GIF |
| `구현 포인트` 아래 | SocketMap으로 중복 배치를 막는 다이어그램 |
| `연결 그래프 기반 지지 판정` 아래 | Anchor 기준 BFS 탐색 후 공중 구조물이 제거되는 GIF |
| `HISM ↔ Actor Hybrid` 아래 | HISM 건축물과 문 Actor가 함께 저장/로드되는 흐름도 |

#### 관련 코드

```text
AChunkManager::AddSocket
AChunkManager::CheckSocket
AChunkManager::AddHISMAttachBuildActor
AChunkManager::RemoveHISMAttachBuildActor
AChunkManager::DamageBuilding
AChunkManager::DestroyBuilding
AChunkManager::DisconnectBuilding
AChunkManager::RemoveOnlyBuilding
AChunkManager::UpgradeBuilding
AChunkManager::UpgradeBuildingDefinitionName
UBuildManagerComponent::FindClosestSocket
ABuildable::Build
ABuildable::BuildDestroy
PP/Buildable/DoorBuildable.cpp
PP/Buildable/OpenDoorBuildable.cpp
PP/Buildable/DestructibleBuildable.cpp
```
---

<a id="data-driven-building"></a>
### 5.5 데이터 기반 건축 시스템과 에셋 로드

#### 문제

건축물 종류가 늘어날수록 클래스, 메시, 내구도, 재질, 배치 조건, 업그레이드 정보를 코드에 직접 연결하기 어렵습니다.  
또 건축물 선택 시 필요한 에셋을 동기 로드하면 순간적인 끊김이 생길 수 있습니다.

#### 구현

`FBuildDefinitionInfo`를 DataTable Row로 정의해 건축물 정보를 데이터화했습니다.  
건축물 클래스, Static Mesh, Destructible Mesh는 Soft Reference로 관리하고, `UBuildAssetSubsystem`에서 필요한 에셋을 비동기로 로드하고 캐싱했습니다.

```cpp
USTRUCT(BlueprintType)
struct FBuildDefinitionInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildType BuildType = EBuildType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EnvironmentsType MaterialType = EnvironmentsType::Wood;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<ABuildable> BuildableClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UStaticMesh> StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UDestructibleMesh> DestructibleMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UpgradeName;
};
```

#### 구현 포인트

- 건축물 클래스와 메시를 Soft Reference로 관리합니다.
- `UBuildAssetSubsystem`에서 필요한 에셋을 비동기로 로드하고 캐싱합니다.
- `UBuildManagerComponent`는 선택된 건축물의 DataTable 정보를 기준으로 프리뷰 Actor를 생성합니다.
- 배치 가능 여부에 따라 성공/실패 머티리얼을 적용합니다.
- 배치 확정 시 정적 건축물은 Actor를 유지하지 않고 Chunk HISM 인스턴스로 전환합니다.

#### 정량 측정 항목

| 항목 | 측정/기록 값 | 설명 |
|---|---:|---|
| Build DataTable Row 수 | [ ] | 등록된 건축물 정의 개수 |
| Soft Reference 최초 로드 시간(ms) | [ ] | 건축물 첫 선택 시 로드 시간 |
| 캐시 재사용 로드 시간(ms) | [ ] | 같은 건축물을 다시 선택했을 때 걸린 시간 |
| 코드 수정 없이 추가 가능한 건축물 수 | [ ] | DataTable Row 추가만으로 확장 가능한 항목 |

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| 구현 아래 | DataTable Row → Build Preview → HISM 등록 흐름도 |
| 구현 포인트 아래 | 건축 프리뷰 생성 및 머티리얼 변경 GIF |
| 측정 항목 아래 | 최초 로드 / 캐시 재사용 시간 비교 그래프 |

#### 관련 코드

```text
PP/GameInfo.h
PP/AssetManager/BuildAssetSubsystem.h
PP/AssetManager/BuildAssetSubsystem.cpp
PP/Buildable/BuildManagerComponent.h
PP/Buildable/BuildManagerComponent.cpp
PP/Buildable/Buildable.h
PP/Buildable/Buildable.cpp
```

---

<a id="inventory-crafting-quest"></a>
### 5.6 인벤토리 / 제작 / 퀘스트 UI

#### 구현 내용

- 인벤토리는 20칸, 퀵슬롯은 6칸 구조로 구현했습니다.
- 아이템 정보는 `FUIItemTableInfo` DataTable에서 읽어옵니다.
- 동일 아이템은 최대 수량까지 스택하고, 초과 수량은 빈 슬롯에 나눠 넣습니다.
- 제작 UI는 무기, 장비, 건축 아이템, 음식 카테고리로 필터링합니다.
- 퀘스트 UI는 사냥/제작 진행도를 갱신하고, 완료 시 보상을 인벤토리에 지급합니다.

#### 정량 측정 항목

| 항목 | 값 | 설명 |
|---|---:|---|
| 인벤토리 슬롯 수 | 20 | 기본 인벤토리 슬롯 |
| 퀵슬롯 수 | 6 | 단축 슬롯 |
| 아이템 DataTable Row 수 | [ ] | 등록된 아이템 수 |
| 제작 가능 아이템 수 | [ ] | Crafting UI에 노출되는 제작 항목 수 |
| 퀘스트 타입 수 | [ ] | 등록된 퀘스트 타입 수 |

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| 구현 내용 아래 | 인벤토리, 퀵슬롯, 제작 UI 캡처 |
| 측정 항목 아래 | 아이템 획득 → 스택 → 제작 → 퀘스트 보상 흐름 GIF |

#### 관련 코드

```text
PP/Widget/InventoryTile.cpp
PP/Widget/UseInventoryTile.cpp
PP/Widget/Crafting.cpp
PP/Widget/QuestWidget.cpp
PP/GameInfo.h
PP/PPGameInstance.cpp
```

---

<a id="ai-combat"></a>
### 5.7 AI 전투 시스템

#### 구현 내용

- 몬스터의 순찰, 추적, 공격, 도주를 Behavior Tree Task로 분리했습니다.
- Player Detect Service에서 거리와 각도를 기준으로 타겟을 감지합니다.
- Decorator로 거리 조건과 보스 Phase 조건을 판단합니다.
- 보스 Morigesh는 Phase에 따라 보호막, 좀비 소환, 폭발, 메테오, 단검 공격 패턴을 사용합니다.

#### 정량 측정 항목

AI는 수치보다 영상/GIF가 더 잘 보입니다. 그래도 수치를 넣는다면 아래 정도가 적당합니다.

| 항목 | 값 | 설명 |
|---|---:|---|
| Behavior Tree Task 수 | [ ] | Patrol, Trace, Attack, Getaway, Boss Phase 등 |
| Service / Decorator 수 | [ ] | 감지, 거리 체크, Phase 체크 등 |
| 보스 Phase 수 | [ ] | Morigesh Phase 패턴 수 |
| 보스 스킬 수 | [ ] | 보호막, 소환, 폭발, 메테오 등 |

#### 이미지/GIF 추천 위치

| 위치 | 추천 자료 |
|---|---|
| 구현 내용 아래 | 일반 몬스터 탐지 → 추적 → 공격 GIF |
| 측정 항목 아래 | Morigesh Phase 전환 및 보스 스킬 사용 GIF |

#### 관련 코드

```text
PP/AI/BTService_PlayerDetect.cpp
PP/AI/BTDecorator_CheckDistance.cpp
PP/AI/BTDecorator_CheckPhase.cpp
PP/AI/BTTask_Patrol.cpp
PP/AI/BTTask_TargetTrace.cpp
PP/AI/BTTask_NormalAttack.cpp
PP/AI/BTTask_Getaway.cpp
PP/AI/BTTask_MorigeshPhase0.cpp
PP/AI/UBTTask_MorigeshPhase1.cpp
PP/AI/UBTTask_MorigeshPhase2.cpp
PP/Character/Monster/Morigesh.cpp
```

---

<a id="code-structure"></a>
## 6. 코드 구조

```text
PP/
├── AI/                  # Behavior Tree Task, Service, Decorator
├── AssetManager/        # 건축 에셋 비동기 로드 및 캐싱
├── Buildable/           # 건축물 Actor, 건축 매니저, 문/벽/창문/천장/계단 등
├── Character/
│   ├── Player/          # 플레이어 이동, 전투, 건축, 인벤토리 연동
│   └── Monster/         # 몬스터, 보스, AI Controller, Anim
├── Chunk/               # 청크, HISM 인스턴싱, 저장/로드, 스트리밍
├── Equipment/           # 무기, 장비, 소비 아이템, 화살 등
├── Environments/        # 나무/돌 등 채집 자원
├── Effect/              # 투사체, 보스 스킬, 실드, 이펙트
├── Tools/               # 벤치마크, 포털, 아이템 박스
└── Widget/              # 인벤토리, 제작, 퀘스트, HUD
```

---

<a id="measurement-method"></a>
## 7. 측정 방법 / 재현 조건

성능 수치를 넣을 때는 숫자만 적기보다 **측정 조건**을 같이 적는 게 좋습니다.  
PC 사양, 해상도, 빌드 타입, 맵 상태에 따라 결과가 달라질 수 있기 때문입니다.

### 공통 측정 환경

| 항목 | 값 |
|---|---|
| CPU | [ ] |
| GPU | [ ] |
| RAM | [ ] |
| OS | [ ] |
| Unreal Engine Version | [엔진 버전 직접 입력] |
| Build Type | Development 패키징 권장, Editor 측정 시 별도 표기 |
| Resolution | [예: 1920x1080] |
| 측정 횟수 | [예: 3~5회 평균] |

### Build Type 기준

최종 README에 넣을 수치는 하나의 빌드 타입을 정해서 여러 번 측정하는 편이 좋습니다.  
포트폴리오용 표라면 Editor보다는 패키징된 Development 또는 Shipping 기준을 추천합니다.

Editor 측정값을 사용할 경우에는 “Editor 기준 측정”이라고 따로 적는 게 좋습니다.  
Editor와 Development/Shipping 결과를 같은 표에 섞으면 비교가 애매해질 수 있습니다.

```text
추천: Development 또는 Shipping 중 하나를 선택해 3~5회 평균 측정
참고: Editor 측정값은 디버깅용으로 분리 표기
```

### 추천 측정 항목

| 구분 | 추천 지표 | README 반영 위치 |
|---|---|---|
| Actor / ISM / HISM 비교 | 생성 시간, FPS, Game/Render/GPU ms, Draw Calls, Actor 수 | `5.1 HISM 기반 청크 월드 관리` |
| 청크 스트리밍 | ActiveChunks, ActiveInstances, ActiveHISMComponents, AvgLoad/Unload/Save ms | `5.2 플레이어 위치 기반 청크 스트리밍` |
| Dirty Save | Save Time, DirtyChunkCount, Save Slot Size | `5.3 Dirty Chunk 기반 저장/로드` |
| 에셋 비동기 로드 | 최초 로드 시간, 캐시 재사용 시간 | `5.5 데이터 기반 건축 시스템과 에셋 로드` |
| 소켓 연결 안정성 | 경계 연결 테스트 수, 연결 유지 성공률, Anchor 기준 파괴 테스트 수 | `5.4 소켓 기반 건축 연결과 HISM ↔ Actor Hybrid` |

### 간단한 측정 코드 예시

```cpp
double StartSeconds = FPlatformTime::Seconds();

RunTargetLogic();

float ElapsedMs = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
UE_LOG(LogTemp, Log, TEXT("[Benchmark] Elapsed: %.3f ms"), ElapsedMs);
```

### 측정 시 주의사항

- 직접 측정하지 않은 수치는 넣지 않습니다.
- Editor와 패키징 빌드 측정값은 구분합니다.
- 평균값을 쓰면 `3회 평균`, `5회 평균`처럼 기준을 같이 적습니다.
- 표는 너무 많이 넣기보다 핵심 비교표 2~3개 정도가 읽기 좋습니다.
- 성능 수치가 어울리지 않는 기능은 GIF나 기능 범위 수치로 보여주는 편이 낫습니다.

---

<a id="tradeoffs"></a>
## 8. 설계상 트레이드오프

### HISM 선택으로 인한 상태 관리 비용

HISM은 대량 오브젝트를 표현하기 좋지만, Actor처럼 각 인스턴스가 자체 상태를 가질 수 없습니다.  
그래서 `BuildingID`, `ResourceID`, `InstanceIndex`, `BuildingMap`, `ResourceMap`을 따로 관리했습니다.

이 방식은 Actor 수를 줄이는 데 유리합니다.  
대신 인스턴스 삭제 후 재매핑, 저장/로드 동기화는 직접 처리해야 합니다.

### 연결 청크 로드로 인한 활성 청크 증가

연결 구조가 끊기지 않도록 기본 스트리밍 반경 밖에 있는 연결 청크도 추가 로드했습니다.  
이 방식은 안정적이지만, 하나의 대형 구조물이 여러 청크에 걸쳐 연결되어 있을 경우 활성 청크 수가 증가할 수 있습니다.

### SaveData 기준 연결 탐색

연결 청크 로드는 Runtime SaveData 캐시를 기준으로 수행합니다.  
그래서 연결 구조가 바뀐 청크는 스트리밍 계산 전에 SaveData 캐시에 반영되도록 처리했습니다.

이 방식은 언로드된 청크의 연결 정보도 탐색할 수 있습니다.  
대신 SaveData 캐시와 런타임 상태가 어긋나지 않게 Dirty Chunk 반영 타이밍을 신경 써야 합니다.

### HISM ↔ Actor Hybrid 구조

현재 구조는 정적 건축물은 HISM으로 관리하고, 문처럼 상호작용이 필요한 일부 오브젝트는 Actor로 유지합니다. 이 방식은 구현 안정성이 높고 현재 프로젝트 범위에서는 Actor 수 증가가 크지 않다는 장점이 있습니다.

다만 상호작용 오브젝트가 많아지면 Actor 수와 별도 저장 데이터가 늘어날 수 있습니다.  
장기적으로는 평상시 HISM으로 두고, 상호작용할 때만 Actor로 전환하는 구조를 검토할 수 있습니다. 이 경우 Transform, Collision, Animation, 저장 데이터, 소켓 상태 동기화가 추가로 필요합니다.

### 동기 로드/언로드

현재 청크 로드/언로드는 동기 방식입니다.  
구현은 단순하지만, 청크 크기와 오브젝트 수가 커지면 순간적인 비용이 생길 수 있습니다.  
추후에는 동기 방식이 아닌 비동기 에셋 로딩방식으로 확장할 수 있습니다.

---

<a id="one-line-summary"></a>
## 9. 한 줄 요약

> 대규모 건축/자원 오브젝트를 HISM과 청크 스트리밍으로 관리하고, ID 기반 상태 관리와 DataTable 기반 건축·아이템·퀘스트 시스템을 결합한 Unreal Engine C++ 클라이언트 포트폴리오입니다.
