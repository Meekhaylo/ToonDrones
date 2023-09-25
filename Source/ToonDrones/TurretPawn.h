// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class TOONDRONES_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	ATurretPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CheckFireConditions();
	void TurnTurret(FVector LookAtTarget);

	UFUNCTION()
	void FireTurret();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_Deviation")
	float Multiplier1;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChassisMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "CPP_Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "CPP_Components")
	TSubclassOf<class ABullet> BulletTemplate;

	class ADronePawn* DroneTarget;

	FTimerHandle FireTimerHandle;
	bool bShouldFire;
	bool bCanFire;
	void FireTurretStart();
	void FireTurretReset();

};
