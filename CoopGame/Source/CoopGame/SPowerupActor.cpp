// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "TimerManager.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerupActor::OnTickPowerUp()
{
	TicksProcessed++;
	OnPowerupTicked();
	if (TotalNumberOfTicks <= TicksProcessed)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(FTimerHandle_PowerupTick);
	}
}

void ASPowerupActor::ActivatePowerup()
{
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(FTimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerUp, PowerupInterval, true, 0.0f);
	}
	else
	{
		OnTickPowerUp();
	}
}
