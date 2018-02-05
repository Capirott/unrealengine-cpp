// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
	bIsPowerupActive = false;
	SetReplicates(true);
}

void ASPowerupActor::OnTickPowerUp()
{
	TicksProcessed++;
	OnPowerupTicked();
	if (TotalNumberOfTicks <= TicksProcessed)
	{
		OnExpired();
		bIsPowerupActive = false;
		OnRep_PowerupActive();
		GetWorldTimerManager().ClearTimer(FTimerHandle_PowerupTick);
	}
}

void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerupActor::ActivatePowerup()
{
	OnActivated();
	bIsPowerupActive = true;
	OnRep_PowerupActive();
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(FTimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerUp, PowerupInterval, true);
	}
	else
	{
		OnTickPowerUp();
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}
