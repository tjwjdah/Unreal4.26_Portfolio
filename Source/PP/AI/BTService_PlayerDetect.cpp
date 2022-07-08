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

	// �ֺ��� �÷��̾ �ִ��� �Ǵ��Ѵ�.

	FVector MonsterLo = Monster->GetActorLocation();

	FCollisionQueryParams	params(NAME_None, false, Monster);

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
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

			// ������ ���⺤�͸� �������ͷ� �����. �� ������ ���� ���·� üũ�� �ϱ� ���ؼ�
			// ������ üũ����� �ϴµ� ���������� ������ �̿��ϰ� �ȴٸ� �� ���Ͱ� ������
			// �ڻ��� ��Ÿ ���� ������ �� �ִ�.
			// �׷��Ƿ� �̷��� �ڻ��� ��Ÿ�� ��ũ�ڻ����� �̿��Ͽ� ��Ÿ(����)�� �����ϰ�
			// �̸� ���Ͽ� ���� �ȿ� ���������� �Ǵ��� �� �ִ�.
			Dir.Normalize();

			FVector	Forward = Monster->GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward);
			// Acos�� �̿��ؼ� ������ ���ϸ� �� ���� Radian ������ ������ �ȴ�.
			// �׷��Ƿ� �̸� Degree �� ��ȯ���ְ� �̸� ���ϴ� ������ ����Ѵ�.
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