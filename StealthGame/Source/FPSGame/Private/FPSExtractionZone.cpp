// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapCom = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapCom->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCom->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapCom->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapCom->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapCom;
	OverlapCom->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
	OverlapCom->SetHiddenInGame(false);
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(OverlapCom);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (!ensure(MyPawn != nullptr)) return;
	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM =  Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
	}
}


