// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(GetRootComponent());
	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;
	InterpTime = 1.f;
	TimeElapsedSinceLastStop = 0.f;
	bShouldInterp = true;
	WaitingTime = 2.f;
	DistanceToTravel = 0.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	DistanceToTravel = EndPoint.Size();
	UnitVectorInDirectionOfTravel = EndPoint.GetSafeNormal();
	EndPoint += StartPoint;
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimeElapsedSinceLastStop >= InterpTime)
	{
		TimeElapsedSinceLastStop = 0;
		SwapVectors(StartPoint, EndPoint);
		bShouldInterp = false;
		GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterp, WaitingTime);
	}
	if (bShouldInterp)
	{
		TimeElapsedSinceLastStop += DeltaTime;
		float DesiredTotalDistance = DistanceToTravel * TimeElapsedSinceLastStop / InterpTime;
		SetActorLocation(StartPoint + UnitVectorInDirectionOfTravel*DesiredTotalDistance);
	}
}

void AFloatingPlatform::SwapVectors(FVector& Vec1, FVector& Vec2)
{
	FVector Temp = Vec1;
	Vec1 = Vec2;
	Vec2 = Temp;
	UnitVectorInDirectionOfTravel *= -1;
}

void AFloatingPlatform::ToggleInterp()
{
	bShouldInterp = !bShouldInterp;
}