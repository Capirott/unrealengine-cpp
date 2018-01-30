// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing (
	TEXT("COOP.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

void ASWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	if (!ensure(MuzzleEffect != nullptr)) return;

	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);

	if (!ensure(TracerEffect != nullptr)) return;

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), TracerEffect, MuzzleLocation);
	if (TracerComp)
	{
		TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
	}
}

void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		const FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
			
			TracerEndPoint = Hit.ImpactPoint;
			
			if (!ensure(ImpactEffect != nullptr)) return;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		if (DebugWeaponDrawing)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			
		}

		PlayFireEffects(TracerEndPoint);
	}
}
