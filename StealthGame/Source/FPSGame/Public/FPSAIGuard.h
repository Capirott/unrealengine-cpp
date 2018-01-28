// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	void StopMovement();

	FRotator OriginalRotation;

	UFUNCTION()
	void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	void MoveToNextPatrolPoint();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChange(EAIState NewState);

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<ATargetPoint*> TargetPoints;

	int8 CurrentTargetIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
