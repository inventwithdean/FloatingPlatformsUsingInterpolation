// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();
	UPROPERTY(VisibleAnywhere, Category=Platform)
	class UStaticMeshComponent* PlatformMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Platform)
	FVector StartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Platform, meta=(MakeEditWidget="true"))
	FVector EndPoint;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Platform)
	float InterpTime;
	UPROPERTY(VisibleInstanceOnly)
	float TimeElapsedSinceLastStop;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Platform)
	bool bShouldInterp;
	FTimerHandle InterpTimer;
	UPROPERTY(EditDefaultsOnly, Category=Platform)
	float WaitingTime;
	UPROPERTY(VisibleAnywhere)
	float DistanceToTravel;
	UPROPERTY()
	FVector UnitVectorInDirectionOfTravel;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SwapVectors(FVector& Vec1, FVector& Vec2);
	UFUNCTION()
	void ToggleInterp();
};
