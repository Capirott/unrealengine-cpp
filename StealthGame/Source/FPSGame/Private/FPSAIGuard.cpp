// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h" 
#include "FPSGameMode.h"
#include "AIController.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
	if (TargetPoints.Num() > 0)
	{
		CurrentTargetIndex = 0;
		MoveToNextPatrolPoint();
	}
	else
	{
		CurrentTargetIndex = -1;
	}
	
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn)
	{ 
		DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(SeenPawn, false);
		}
	}
	SetGuardState(EAIState::Alerted);
	StopMovement();
}

void AFPSAIGuard::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, 10.0f);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;

	SetActorRotation(NewLookAt);

	
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);
	StopMovement();
}

void AFPSAIGuard::StopMovement()
{
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
	MoveToNextPatrolPoint();
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnStateChange(NewState);
	
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentTargetIndex != -1)
	{
		CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetPoints.Num();
		ATargetPoint* Target = TargetPoints[CurrentTargetIndex];
		if (!ensure(Target != nullptr)) return;

		UNavigationSystem::SimpleMoveToActor(GetController(), Target);
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTargetIndex != -1)
	{
		if (!ensure(TargetPoints[CurrentTargetIndex] != nullptr)) return;

		FVector Delta = GetActorLocation() - TargetPoints[CurrentTargetIndex]->GetTargetLocation();

		float DistanceToGoal = Delta.Size();
		UE_LOG(LogTemp, Error, TEXT("The value of 'Delta' is: %f"), DistanceToGoal);
		if (DistanceToGoal < 100)
		{
			MoveToNextPatrolPoint();
		}
	}
	
}
