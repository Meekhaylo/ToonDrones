// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONDRONES_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	void SetHealth(int32 HPAmount);
	void HealthPickedUp(int32 Amount, AActor* HealedActor);

	UPROPERTY(EditAnywhere)
	int32 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentHealth = 100;
private:

	UFUNCTION()
	void DamageTaken(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		class AController* Instigator, /*Instigator - Controller responsible for the damage. If player is possessing a pawn,
		than it means controller which posseses that pawn*/
		AActor* DamageCauser /*In this case it means bullet (or projectile)*/
	);

	

	
};
