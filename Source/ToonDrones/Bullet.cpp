#include "Bullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetStringLibrary.h>

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	RootComponent = BulletMesh;

	SmokeTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	SmokeTrail->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile movement component"));
	ProjectileMovementComponent->InitialSpeed = 10000.f;
	ProjectileMovementComponent->MaxSpeed = 10000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SmokeTrail->HasCompleted())
	{
		Destroy();
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	AActor* BulletInstigator = GetOwner();
	if (BulletInstigator == nullptr)
	{
		//SmokeTrail->Deactivate();
		//BulletMesh->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("No bullet instigator"));
		Destroy();
		return;
	}

	AController* BulletInstigatorController = BulletInstigator->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();


	if (OtherActor && OtherActor != this && OtherActor != BulletInstigator)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, BulletInstigatorController, this, DamageTypeClass);
		
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}

		//Destroy();
	}
	
	SmokeTrail->Deactivate();
	BulletMesh->SetVisibility(false);
	BulletMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Destroy();
}

