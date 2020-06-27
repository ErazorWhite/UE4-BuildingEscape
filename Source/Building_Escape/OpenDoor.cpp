// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the \"OpenDoor\" component on it, but no \"PressurePlate\" set!"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * 1.f); // Just another way to change Yaw
	// CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, 45); // constant way to change Yaw
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpeningSpeed); // Flexible way to change Yaw
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * 1.0f); // Just another way to change Yaw
	// CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, 45); // constant way to change Yaw
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed); // Flexible way to change Yaw
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find All Overlapping Actors.
	TArray<AActor *> OverlappingActors;
	
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the \"OpenDoor\" component on it, but no \"PressurePlate\" set!"), *GetOwner()->GetName())
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}