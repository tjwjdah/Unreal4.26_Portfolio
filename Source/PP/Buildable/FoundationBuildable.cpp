// Fill out your copyright notice in the Description page of Project Settings.


#include "FoundationBuildable.h"

AFoundationBuildable::AFoundationBuildable() {
	PrimaryActorTick.bCanEverTick = true;

	
	USceneComponent* BuildComponents = CreateDefaultSubobject<USceneComponent>(TEXT("BuildComponents"));
	SceneComponents.Add(BuildComponents);

	USceneComponent* FoundationSockets = CreateDefaultSubobject<USceneComponent>(TEXT("FoundationSockets"));
	SceneComponents.Add(FoundationSockets);
	USceneComponent* FoundationSocketUp     = CreateDefaultSubobject<USceneComponent>(TEXT("FoundationSocketUp"));
	SceneComponents.Add(FoundationSocketUp);
	USceneComponent* FoundationSocketDown   = CreateDefaultSubobject<USceneComponent>(TEXT("FoundationSocketDown"));
	SceneComponents.Add(FoundationSocketDown);
	USceneComponent* FoundationSocketRight  = CreateDefaultSubobject<USceneComponent>(TEXT("FoundationSocketRight"));
	SceneComponents.Add(FoundationSocketRight);
	USceneComponent* FoundationSocketLeft   = CreateDefaultSubobject<USceneComponent>(TEXT("FoundationSocketLeft"));
	SceneComponents.Add(FoundationSocketLeft);

	USceneComponent* TriangleFoundationSockets = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleFoundationSockets"));
	SceneComponents.Add(TriangleFoundationSockets);
	USceneComponent* TriangleFoundationSocketUp    = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleFoundationSocketUp"));
	SceneComponents.Add(TriangleFoundationSocketUp);
	USceneComponent* TriangleFoundationSocketDown  = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleFoundationSocketDown"));
	SceneComponents.Add(TriangleFoundationSocketDown);
	USceneComponent* TriangleFoundationSocketRight = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleFoundationSocketRight"));
	SceneComponents.Add(TriangleFoundationSocketRight);
	USceneComponent* TriangleFoundationSocketLeft  = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleFoundationSocketLeft"));
	SceneComponents.Add(TriangleFoundationSocketLeft);


	USceneComponent* WallSockets = CreateDefaultSubobject<USceneComponent>(TEXT("WallSockets"));
	SceneComponents.Add(WallSockets);
	USceneComponent* WallSocketUp    = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocketUp"));
	SceneComponents.Add(WallSocketUp);
	USceneComponent* WallSocketDown  = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocketDown"));
	SceneComponents.Add(WallSocketDown);
	USceneComponent* WallSocketRight = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocketRight"));
	SceneComponents.Add(WallSocketRight);
	USceneComponent* WallSocketLeft  = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocketLeft"));
	SceneComponents.Add(WallSocketLeft);

	USceneComponent* RampSockets = CreateDefaultSubobject<USceneComponent>(TEXT("RampSockets"));
	SceneComponents.Add(RampSockets);
	USceneComponent* RampSocketUp    = CreateDefaultSubobject<USceneComponent>(TEXT("RampSocketUp"));
	SceneComponents.Add(RampSocketUp);
	USceneComponent* RampSocketDown  = CreateDefaultSubobject<USceneComponent>(TEXT("RampSocketDown"));
	SceneComponents.Add(RampSocketDown);
	USceneComponent* RampSocketRight = CreateDefaultSubobject<USceneComponent>(TEXT("RampSocketRight"));
	SceneComponents.Add(RampSocketRight);
	USceneComponent* RampSocketLeft  = CreateDefaultSubobject<USceneComponent>(TEXT("RampSocketLeft"));
	SceneComponents.Add(RampSocketLeft);

	USceneComponent* StairsSocket = CreateDefaultSubobject<USceneComponent>(TEXT("StairsSocket"));
	SceneComponents.Add(StairsSocket);

	USceneComponent* FloorSocket = CreateDefaultSubobject<USceneComponent>(TEXT("FloorSocket"));
	SceneComponents.Add(FloorSocket);

	USceneComponent* RoofSocket = CreateDefaultSubobject<USceneComponent>(TEXT("RoofSocket"));
	SceneComponents.Add(RoofSocket);


	BuildComponents->SetupAttachment(RootComponent);

	FoundationSockets->SetupAttachment(BuildComponents);
	FoundationSocketUp->SetupAttachment(FoundationSockets);
	FoundationSocketDown->SetupAttachment(FoundationSockets);
	FoundationSocketRight->SetupAttachment(FoundationSockets);
	FoundationSocketLeft->SetupAttachment(FoundationSockets);

	
	TriangleFoundationSockets->SetupAttachment(BuildComponents);
	TriangleFoundationSocketUp->SetupAttachment(TriangleFoundationSockets);
	TriangleFoundationSocketDown->SetupAttachment(TriangleFoundationSockets);
	TriangleFoundationSocketRight->SetupAttachment(TriangleFoundationSockets);
	TriangleFoundationSocketLeft->SetupAttachment(TriangleFoundationSockets);
	
	WallSockets->SetupAttachment(BuildComponents);
	WallSocketUp->SetupAttachment(WallSockets);
	WallSocketDown->SetupAttachment(WallSockets);
	WallSocketRight->SetupAttachment(WallSockets);
	WallSocketLeft->SetupAttachment(WallSockets);
	
	RampSockets->SetupAttachment(BuildComponents);
	RampSocketUp->SetupAttachment(RampSockets);
	RampSocketDown->SetupAttachment(RampSockets);
	RampSocketRight->SetupAttachment(RampSockets);
	RampSocketLeft->SetupAttachment(RampSockets);

	StairsSocket->SetupAttachment(BuildComponents);

	FloorSocket->SetupAttachment(BuildComponents);

	RoofSocket->SetupAttachment(BuildComponents);

	SceneComponents[(int32)EFoundationSocket::FoundationSocketDown]->SetWorldLocation(FVector(0.f, -300.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::FoundationSocketUp]->SetWorldLocation(FVector(0.f, 300.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::FoundationSocketLeft]->SetWorldLocation(FVector(300.f, 0.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::FoundationSocketRight]->SetWorldLocation(FVector(-300.f, 0.f, 0.f));

	SceneComponents[(int32)EFoundationSocket::TriangleFoundationSocketDown]->SetWorldLocation(FVector(0.f, -236.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::TriangleFoundationSocketUp]->SetWorldLocation(FVector(0.f, 236.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::TriangleFoundationSocketLeft]->SetWorldLocation(FVector(236.f, 0.f, 0.f));
	SceneComponents[(int32)EFoundationSocket::TriangleFoundationSocketRight]->SetWorldLocation(FVector(-236.f, 0.f, 0.f));



	SceneComponents[(int32)EFoundationSocket::WallSocketDown]->SetWorldLocation(FVector(0.f, -150.f, 235.f));


	SceneComponents[(int32)EFoundationSocket::WallSocketUp]->SetWorldLocation(FVector(0.f, 150.f, 235.f));
	
	SceneComponents[(int32)EFoundationSocket::WallSocketLeft]->SetWorldLocation(FVector(150.f, 0.f, 235.f));
	
	SceneComponents[(int32)EFoundationSocket::WallSocketRight]->SetWorldLocation(FVector(-150.f, 0.f, 235.f));
	
	SceneComponents[(int32)EFoundationSocket::RampSocketDown]->SetWorldLocation(FVector(0.f, -300.f, 0.f));
	
	SceneComponents[(int32)EFoundationSocket::RampSocketLeft]->SetWorldLocation(FVector(300.f, 0.f, 0.f));
	
	SceneComponents[(int32)EFoundationSocket::RampSocketRight]->SetWorldLocation(FVector(-300.f, 0.f, 0.f));
	
	SceneComponents[(int32)EFoundationSocket::RampSocketUp]->SetWorldLocation(FVector(0.f, 300.f, 0.f));
	
	SceneComponents[(int32)EFoundationSocket::StairsSocket]->SetWorldLocation(FVector(0.f, 0.f, 245.f));

	SceneComponents[(int32)EFoundationSocket::FloorSocket]->SetWorldLocation(FVector(0.f, 0.f, 85.f));

	SceneComponents[(int32)EFoundationSocket::RoofSocket]->SetWorldLocation(FVector(0.f, 0.f, 235.f));

	PreBuildCheck = false;
}

void AFoundationBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AFoundationBuildable::Build()
{
	Super::Build();
}
void AFoundationBuildable::BeginPlay()
{
	
	Super::BeginPlay();


}
void AFoundationBuildable::BuildChecking(FVector Location)
{
	
	
	ResultLocation = Location;
	HitAmount = 0;
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	
	TArray<FHitResult>	resultArray;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		Location, Location, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(ActorSize),
		params);
	
	if (Hit)
	{
		
		for (auto& result : resultArray) {
			if (result.Actor->IsA(AFoundationBuildable::StaticClass()))
			{
				AFoundationBuildable* Act = Cast<AFoundationBuildable>(result.Actor);
				FVector Lot = result.Location- Act->GetSceneComponent((int32)EFoundationSocket::FoundationSocketUp)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EFoundationSocket::FoundationSocketUp)->GetComponentLocation();
				for (int32 i = (int32)EFoundationSocket::FoundationSocketDown; i <= (int32)EFoundationSocket::FoundationSocketLeft; i++)
				{
					Lot = result.Location - Act->GetSceneComponent(i)->GetComponentLocation();
					float Length2 = Lot.Size();
					if (Length1 > Length2)
					{
						Length1 = Length2;
						ResultLocation = Act->GetSceneComponent(i)->GetComponentLocation();
					}
				}
			
				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
				
				
			}
			else
			{
				ResultLocation.Z = Location.Z + ActorSize.Z;
			}
		}
		bool Hit2 = GetWorld()->SweepMultiByChannel(resultArray,
			ResultLocation, ResultLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeBox(ActorSize),
			params);
		for (auto& result : resultArray) {
			if (result.Actor->IsA(AFoundationBuildable::StaticClass()))
			{
				//float dis = FVector::Distance(ResultLocation, result.Actor->GetActorLocation());
				if (result.PenetrationDepth > 0.1f)
				{
					HitAmount++;
				}
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), dis));
			}
		}
		
	}
	
	if (HitAmount >= 2)
	{
		BuildCheck = false;
	}
	else
	{
		BuildCheck = true;
	}
	//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("%f %f %f"), ActorSize.X, ActorSize.Y, ActorSize.Z));
	SetActorLocation(ResultLocation);
	Super::BuildChecking(ResultLocation);
}
