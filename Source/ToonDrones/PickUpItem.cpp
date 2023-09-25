// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "Components/SphereComponent.h"
#include "DronePawn.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

APickUpItem::APickUpItem() :
	bAmmoPickUp(false),
	bHealthPickUp(false)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnSphereOverlap); // Using a dynamic delegate. It has a struct
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &APickUpItem::OnSphereEndOverlap);
}

void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin overlap"));
	if (OtherActor)
	{
		ADronePawn* DronePawn = Cast<ADronePawn>(OtherActor);
		if (DronePawn)
		{
			if (bHealthPickUp)  
			{
				if (DronePawn->GetCurrentDroneHP() < 150)
				{
					if (UHealthComponent* TemComp = DronePawn->GetComponentByClass<UHealthComponent>())
					{
						TemComp->HealthPickedUp(50, OtherActor);
						Destroy();
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Health is full"));
				}
			}
			if (bAmmoPickUp)
			{
				DronePawn->PickUpAmmo();
				Destroy();
			}
			if (bHealthPickUp == false || bAmmoPickUp == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("No pick up type is designated"));
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("DronePawn overlap"));
	}
}

void APickUpItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End overlap"));
}

