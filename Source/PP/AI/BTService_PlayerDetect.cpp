// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Monster.h"
#include "../Character/Monster/MonsterAIController.h"


const float DetectAngle = 150.f;

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}


void UBTService_PlayerDetect::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return;
	
	if (Monster->GetWait())
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		return;
	}
	
	
	
	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 주변에 플레이어가 있는지 판단한다.

	FVector MonsterLo = Monster->GetActorLocation();

	FCollisionQueryParams	params(NAME_None, false, Monster);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	HitResult;
	bool Sweep = GetWorld()->SweepSingleByChannel(HitResult,
		MonsterLo,
		MonsterLo, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(MonsterInfo.TraceDistance),
		params);
	if (Monster->GetAttackPlayer() == nullptr)
	{
		//PrintViewport(10.f, FColor::Blue, Monster->GetName());
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("nullptr")));
	FHitResult	HitResult2;
	
		if (Sweep)
		{

			FVector	Dir = HitResult.ImpactPoint - MonsterLo;

			// 구해준 방향벡터를 단위벡터로 만든다. 그 이유는 원뿔 형태로 체크를 하기 위해서
			// 각도를 체크해줘야 하는데 단위벡터의 내적을 이용하게 된다면 더 벡터간 각도의
			// 코사인 세타 값을 구해줄 수 있다.
			// 그러므로 이러한 코사인 세타를 아크코사인을 이용하여 세타(각도)로 변경하고
			// 이를 비교하여 원뿔 안에 들어오는지를 판단할 수 있다.
			Dir.Normalize();

			FVector	Forward = Monster->GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward);
			// Acos를 이용해서 각도를 구하면 이 값은 Radian 값으로 들어오게 된다.
			// 그러므로 이를 Degree 로 변환해주고 이를 비교하는 각도로 사용한다.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle);
			/*
#if ENABLE_DRAW_DEBUG

			FColor	DrawColor = Angle <= DetectAngle ? FColor::Red : FColor::Green;

			DrawDebugCone(GetWorld(), MonsterLo, Forward, MonsterInfo.TraceDistance,
				FMath::DegreesToRadians(DetectAngle),
				FMath::DegreesToRadians(DetectAngle), 20,
				DrawColor, false, 1.f);

#endif
*/
			if (Angle <= DetectAngle)
			{
				Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());
				Monster->SetAttackPlayer(HitResult.GetActor());
			}
			else
			{
				Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
			}

		}
		else
		{
			/*
#if ENABLE_DRAW_DEBUG

			FColor	DrawColor = FColor::Green;
			FVector	Forward = Monster->GetActorForwardVector();
			DrawDebugCone(GetWorld(), MonsterLo, Forward, MonsterInfo.TraceDistance,
				FMath::DegreesToRadians(DetectAngle),
				FMath::DegreesToRadians(DetectAngle), 20,
				DrawColor, false, 1.f);

#endif
*/
			Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);

		}

	}
	else
	{
		//PrintViewport(10.f, FColor::Blue, Monster->GetName());
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("!!nullptr")));
		/*
#if ENABLE_DRAW_DEBUG

		FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

		DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
			MonsterInfo.TraceDistance, 20, DrawColor, false,
			0.3f);

#endif
*/
		if(Monster->GetHit())
		{
			Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Monster->GetAttackPlayer());
		}
		else 
		{
			if (Sweep)
				Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());

			else
			{
				Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
				Monster->SetAttackPlayer(nullptr);
			}
		}
	}

}