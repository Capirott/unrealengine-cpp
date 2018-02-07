// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SHealthComponent.h"
#include "SGameState.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	GameStateClass = ASGameState::StaticClass();
}

void ASGameMode::StartWave()
{
	++WaveCount;
	NrOfBotToSpawn = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
	SetWaveState(EWaveState::WaitingToStart);
}

void ASGameMode::CheckWaveState()
{
	const bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);	
	if (NrOfBotToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}
	bool bIsAnyBotAlive = false;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}

	}
	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
		SetWaveState(EWaveState::WaveComplete);
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{ 
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator() ; It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GameState = GetGameState<ASGameState>();
	if (!ensure(GameState != nullptr)) return;
	GameState->WaveState = NewState;
}


void ASGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	--NrOfBotToSpawn;
	if (NrOfBotToSpawn <= 0)
	{
		EndWave();
	}
}
