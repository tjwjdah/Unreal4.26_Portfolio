// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "../Character/Player/PlayerCharacter.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));


	m_Box->SetWorldScale3D(FVector(100.f, 100.f, 100.f));
	m_Box->OnComponentBeginOverlap.AddDynamic(this, &APortal::OverlapBegins);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APortal::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("asdsada")));
	if (OtherActor == MyCharacter)
	{
		
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, TEXT("MerlinsCave"), true, true, LatentInfo);

		OtherActor->SetActorLocation(FVector(1296.f, 118.999992f, 151.f));
		OtherActor->SetActorRotation(FRotator(0.f, 180.f, 0.f));

		//FLatentActionInfo LatentInfo2;
	

		//UGameplayStatics::UnloadStreamLevel(this, TEXT("Island"), LatentInfo2,false);


		FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading.Loading_C'"));
		UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>();
		UUserWidget* UIValue = CreateWidget<UUserWidget>(GetWorld(), MyWidgetClass);
		UIValue->AddToViewport();


	}

}

