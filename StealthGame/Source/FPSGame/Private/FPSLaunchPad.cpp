// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaunchBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerSphereComp"));
	LaunchBoxComponent->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = LaunchBoxComponent;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	LaunchBoxComponent->SetupAttachment(LaunchBoxComponent);


	LaunchBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchBox);

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;

}


void AFPSLaunchPad::OverlapLaunchBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDiretion = GetActorRotation();
	LaunchDiretion.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDiretion.Vector() * LaunchStrength;
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else
	{
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
		}
	}
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

