#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"


//#include "../../../../Program Files/Epic Games/UE_5.2/Engine/Plugins/Runtime/nDisplay/Source/DisplayClusterProjection/Public/Blueprints/IDisplayClusterProjectionBlueprintAPI.h"

UCLASS()
class TOONDRONES_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	ADronePawn();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Functions
	FORCEINLINE void PickUpAmmo() { Ammo += 5; }
	FORCEINLINE void PickUpHP() { HP += 20; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAmmo() { return Ammo ; }

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentDroneHP();
	void SetCurrentDroneHP(int32 Val);
		
private:
	// Input properties
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* SCMapCont;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IALook;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAFire;

	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire();

	// Pawn properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChassisMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "CPP_Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "CPP_Components")
	class UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, Category = "CPP_Components")
	class UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPP_Components")
	TSubclassOf<class ABullet> BulletTemplate;

	//In game properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP_Components", meta = (AllowPrivateAccess = "true"))
	int32 HP;

	



};
