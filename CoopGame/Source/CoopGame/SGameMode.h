// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_BotSpawner;
	
	FTimerHandle TimerHandle_NextWaveStart;

	int32 NrOfBotToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();
		
	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void CheckWaveState();


public:

	ASGameMode();

	void StartPlay() override;

	void Tick(float DeltaSeconds) override;
};
