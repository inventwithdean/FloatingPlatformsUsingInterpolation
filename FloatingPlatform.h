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

	/** Mesh of the Platform */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* PlatformMesh;

	/** Start position of Platform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	FVector StartPos;

	/** End position of Platform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform", meta = (MakeEditWidget = "true"))
	FVector EndPos;

	/** Bool to keep track of whether Platform is interpolating or not */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Platform")
	bool bIsInterping;

	/** Floating Platform will take *InterpDuration seconds to reach next position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	float InterpDuration;

	/** Time in Seconds to pause at Start and End positions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	float LerpPause;

	/** Float to keep track of Time elapsed in seconds
	/** It is being reset at end of each positions to make platform back and forth */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	float TimeElapsed;

	// *** IMPORTANT ***
	// It can't have any Macro Specifiers!
	UPROPERTY()
	FTimerHandle PlatformTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Function which will be called by Timer to Toggle bIsInterping bool so that it can be used to move platform */
	void ToggleInterping();

	/** Function to swap to vectors, which will help in swapping start and end positions */
	void SwapVectors(FVector& VecOne, FVector& VecTwo);
};
