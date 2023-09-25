// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include <Components/CapsuleComponent.h>
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>
#include "DronePawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HealthComponent.h"

ATurretPawn::ATurretPawn() :
	bShouldFire(false),
	bCanFire(true),
	Multiplier1(0.0001f)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	ChassisMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(ChassisMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	DroneTarget = Cast<ADronePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Multiplier1 = 0.0001f;
	HealthComponent->SetHealth(30);
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFireConditions();
	if (bShouldFire)
	{
		FireTurretStart();
	}
}

void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretPawn::CheckFireConditions()
{
	if (DroneTarget)
	{
		FVector CrosshairTraceStart{ GetActorLocation() };
		CrosshairTraceStart.Z += 50;
		FVector CrosshairTraceEnd{ DroneTarget->GetActorLocation() };
		CrosshairTraceEnd.Z -= 100;


		FHitResult OUTHitResult;
		DrawDebugLine(GetWorld(), CrosshairTraceStart, CrosshairTraceEnd, FColor::Red, false, 0.1f);
		DrawDebugLine(GetWorld(), CrosshairTraceStart, DroneTarget->GetActorLocation(), FColor::Green, false, 0.1f);
		GetWorld()->LineTraceSingleByChannel(OUTHitResult, CrosshairTraceStart, DroneTarget->GetActorLocation(), ECollisionChannel::ECC_Visibility);
		
		if (OUTHitResult.GetActor() == DroneTarget)
		{
			bShouldFire = true;

			FString Name;
			OUTHitResult.GetActor()->GetName(Name);
			//UE_LOG(LogTemp, Warning, TEXT("Drone target spotted, %s"), *Name);

			float DistanceToTarget = FVector::Dist(GetActorLocation(), DroneTarget->GetActorLocation());
			FVector TargetVelocity = DroneTarget->GetVelocity() * DistanceToTarget * Multiplier1;
			CrosshairTraceEnd += TargetVelocity;

			TurnTurret(CrosshairTraceEnd);
			
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("No target spotted"));
			bShouldFire = false;
		}
			
	}
	else 
	{
		bShouldFire = false;
	}
}

void ATurretPawn::TurnTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - (TurretMesh->GetComponentLocation());
	//FRotator Rotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	FRotator Rotation = FRotator(ToTarget.Rotation().Pitch, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			Rotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			10
		)
	);

}

void ATurretPawn::FireTurret()
{
	ABullet* TempBulletOwner = GetWorld()->SpawnActor<ABullet>(BulletTemplate, ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	TempBulletOwner->SetOwner(this);
	
}

void ATurretPawn::FireTurretStart()
{
	if (bCanFire)
	{
		FireTurret();
		bCanFire = false;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurretPawn::FireTurretReset, 2.f, false);
	}
}

void ATurretPawn::FireTurretReset()
{
	bCanFire = true;
}

