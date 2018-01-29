// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SProjectile.h"

void ASProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		if (!ensure(MuzzleEffect != nullptr)) return;

		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);

		if (!ensure(TracerEffect != nullptr)) return;

		if (!ensure(ProjectileClass != nullptr)) return;

		if (ProjectileClass)
		{
			const FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			const FRotator MuzzleRotation = EyeRotation;

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			// spawn the projectile at the muzzle

			GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		}
	}
}

