#include "DronePawn.h"
//#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
//#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../../../../Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "../../../../Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include <Kismet/KismetStringLibrary.h>
#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include "Bullet.h"
#include "HealthComponent.h"

ADronePawn::ADronePawn() :
	Ammo(0),
	HP(0)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	ChassisMesh->SetupAttachment(CapsuleComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(ChassisMesh);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("UFloatingPawnMovement"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(SCMapCont, 0);
		}
	}
	
	Ammo = 25;
	HP = 100;
}

void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{	
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &ADronePawn::Move);
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &ADronePawn::Look);
		EnhancedInputComponent->BindAction(IAFire, ETriggerEvent::Triggered, this, &ADronePawn::Fire);
	}
}

int32 ADronePawn::GetCurrentDroneHP()
{
	return HP;
}

void ADronePawn::SetCurrentDroneHP(int32 Val)
{
	HP = Val;
}

void ADronePawn::Move(const FInputActionValue& Value)
{
	//FString InputValue{ FString::Printf(TEXT("Move input: %s"), *Value.ToString()) };
	//GEngine->AddOnScreenDebugMessage(5, 0.f, FColor::White, InputValue);
	
	const FVector MoveDirection = Value.Get<FVector>();
	if (GetController())
	{
		FVector Forward{ GetActorForwardVector() };
		Forward.Z = 0;
		AddMovementInput(Forward, MoveDirection.X);

		FVector Right{ GetActorRightVector() };
		Right.Z = 0;
		AddMovementInput(Right, MoveDirection.Y);

		AddMovementInput(FVector().UpVector, MoveDirection.Z);
	}
}

void ADronePawn::Look(const FInputActionValue& Value)
{
	//FString InputValue{ FString::Printf(TEXT("Look input: %s"), *Value.ToString()) };
	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, InputValue);

	const FVector2D LookDirection = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void ADronePawn::Fire()
{
	if (Ammo)
	{
		ABullet* TempBulletOwner = GetWorld()->SpawnActor<ABullet>(BulletTemplate, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		TempBulletOwner->SetOwner(this);
		Ammo--;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning! Out of ammo"));
	}
	
}

