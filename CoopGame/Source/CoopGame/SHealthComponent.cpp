#include "SHealthComponent.h"
#include "GameFramework/Actor.h"


USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.0f;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}	
	Health = DefaultHealth;

}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	
	UE_LOG(LogTemp, Log, TEXT("Health Changed %s"), *FString::SanitizeFloat(Health));
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);	
}
