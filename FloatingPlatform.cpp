// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating a root component of TYPE USceneComponent
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

	// Creating Platform mesh and attaching it to Root Component
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(GetRootComponent());

	// Initialising properties
	StartPos = FVector(0);
	EndPos = FVector(0);
	bIsInterping = false;
	InterpDuration = 3.0f;
	TimeElapsed = 0.f;
	LerpPause = 3.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Setting StartPos to be starting location of this actor
	StartPos = GetActorLocation();

	// Adding StartPos to EndPos to make EndPos a global Vector rather than before when it was in local space
	// It was relative to the Root Component of this Actor.
	// You can skip this step if you don't have MakeEditWidget meta specifier in UPROPERTY macro of EndPos FVector
	EndPos += StartPos;

	// Set Initial Timer for Floating Platform equal to the LerpPause
	GetWorldTimerManager().SetTimer(PlatformTimer, this, &AFloatingPlatform::ToggleInterping, LerpPause);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimeElapsed <= InterpDuration && bIsInterping)
	{
		TimeElapsed += DeltaTime;

		// TimeElapsed/InterpDuration is making sure that alpha ranges only between 0 and 1
		// When TimeElapsed will be equal to InterpDuration, alpha will be 1, i.e., Platform 
		// will be at endPos which is what we want
		FVector NewLocation = FMath::Lerp(StartPos, EndPos, TimeElapsed/InterpDuration);

		// Setting location of Platform Mesh rather than setting SetActorLocation which will set location of Root Component.
		// It makes sure that endPos is not moved which won't be moved in first case either but you know to be sure
		// As if you used SetActorLocation, endPos' widget will also move. You can see it in editor
		// *** IMPORTANT It is setting world location of the Platform.
		// So suppose Actor was placed at (0, 0, 0) and then rotated 180 degree along z axis and you place its endPos 1000 units in front of Platform.
		// You will see +1000 in X component of EndPos in the editor, i.e., when it will add StartPos to EndPos,
		// It will treat as if its direction was reversed. i.e., EndPos will be (1000, 0, 0) even if globally, 
		// it was supposed to be (-1000, 0, 0), I want to say that rotation of actor won't matter. It would still
		// treat endPos as if RootComponent's rotation was reset to (0, 0, 0) then it will add RootComponent's Location 
		// to EndPos
		// So make sure that your Actor's local axis are aligned with Global Axis
		PlatformMesh->SetWorldLocation(NewLocation);
		
	}

	// Checking if velocity of Platform is 0 AND Platform is currently interping.
	// ***IMPORTANT***
	// else if IS NECESSARY BECAUSE 
	// IF IT WAS ONLY A if STATEMENT, THEN IT COULD ALSO BE CALLED AFTER FIRST 'if' STATMENT IF
	// BY ANY CHANCE VELOCITY WOULD BE 0. (I KNOW IT WON'T BE ZERO BUT IN CASE IF LERP DURATION WAS
	// TOO BIG BECAUSE OF END POSITION BEING TOO FAR AWAY, THEN VELOCITY AFTER FIRST FRAME COULD BE
	// ZERO)
	// In case you are wondering how I'm so sure that velocity vector's magnitude will be zero. It will because when
	// the above 'if' statment won't be called then position of platform won't change, Hence it will be at rest(as there
	// is no PHYSICS applied)
	else if (PlatformMesh->ComponentVelocity.Size() == 0.f && bIsInterping)
	{
		// Setting bIsInterping to false so that this 'else if' code block is not called until platform reaches next position
		bIsInterping = false;

		// Setting TimeElapsed to be 0 because the first 'if' condition of Tick function uses it
		TimeElapsed = 0.f;

		// Swapping StartPos and EndPos vectors
		SwapVectors(StartPos, EndPos);

		// Setting Timer of *LerpPause seconds to call *ToggleInterping function which then sets bIsInterping 
		// to be true so that 'if' condition can execute
		GetWorldTimerManager().SetTimer(PlatformTimer, this, &AFloatingPlatform::ToggleInterping, LerpPause);
	}
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	// Creating a Temporary vector which stores value of VecOne Vector
	FVector Temp = VecOne;

	// Assigning value of VecTwo vector to VecOne vector, i.e., Now first vector holds the value of second vector
	VecOne = VecTwo;

	// Assigning value of Temp Vector(VecOne Vector) to VecTwo vector,
	// Now second vector holds the original value of first vector
	VecTwo = Temp;
}

void AFloatingPlatform::ToggleInterping()
{
	// You can also set it to true in this case because in Tick Function only true condition is checked
	bIsInterping = !bIsInterping; 
}