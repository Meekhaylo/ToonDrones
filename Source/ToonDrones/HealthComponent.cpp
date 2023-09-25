#include "HealthComponent.h"
#include "DronePawn.h"

UHealthComponent::UHealthComponent() :
	CurrentHealth(0)
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	CurrentHealth = MaxHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::SetHealth(int32 HPAmount)
{
	MaxHealth = HPAmount;
	CurrentHealth = HPAmount;
}

void UHealthComponent::HealthPickedUp(int32 Amount, AActor* HealedActor)
{
	CurrentHealth += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Health component, HealthPickedUp trigerred"));
	if (ADronePawn* TempPawn = Cast<ADronePawn>(HealedActor))
	{
		TempPawn->SetCurrentDroneHP(CurrentHealth);
	}
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Health component, DamageTaken trigerred"));
	if (Damage <= 0.f) 
	{
		return;
	}

	CurrentHealth -= Damage;
	if (ADronePawn* TempPawn = Cast<ADronePawn>(DamagedActor))
	{
		TempPawn->SetCurrentDroneHP(CurrentHealth);
	}

	if (CurrentHealth <= 0)
	{
		DamagedActor->Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("DamageTaken! Current health is %d"), CurrentHealth);
}



