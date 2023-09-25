#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TOONDRONES_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditDefaultsOnly, Category = "CPP_Components")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, Category = "CPP_Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "CPP_Components")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "CPP_Components")
	class UParticleSystemComponent* SmokeTrail;

	UPROPERTY(EditAnywhere)
	float Damage = 15.f;
};
