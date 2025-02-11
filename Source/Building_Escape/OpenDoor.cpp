// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
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
	InitDoorLocation = GetOwner()->GetActorLocation().Z;
	CurrentDoorLocation = InitDoorLocation;
	OpenedDoorLocation += InitDoorLocation;

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
		return;
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the \"OpenDoor\" component on it, but no \"PressurePlate\" set!"), *GetOwner()->GetName())
		return;
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
	// CurrentVector = FMath::Lerp(CurrentVector, OpenVector, DeltaTime * 1.f); // Just another way to change Yaw
	// CurrentVector = FMath::FInterpConstantTo(CurrentVector, OpenVector, DeltaTime, 45); // constant way to change Yaw
	// CurrentVector = FMath::FInterpTo(CurrentVector, OpenVector, DeltaTime, DoorOpeningSpeed); // Flexible way to change Yaw

	CurrentDoorLocation = FMath::FInterpConstantTo(CurrentDoorLocation, OpenedDoorLocation, DeltaTime, DoorOpeningSpeed); // Flexible way to change Yaw
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = CurrentDoorLocation;

	GetOwner()->SetActorLocation(DoorLocation);

	if (!AudioComponent)
	{
		return;
	}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// CurrentVector = FMath::Lerp(CurrentVector, InitVector, DeltaTime * 1.0f); // Just another way to change Yaw
	// CurrentVector = FMath::FInterpConstantTo(CurrentVector, InitVector, DeltaTime, 45); // constant way to change Yaw
	// CurrentVector = FMath::FInterpConstantTo(CurrentVector, OpenVector, DeltaTime, 45); // constant way to change Yaw
	CurrentDoorLocation = FMath::FInterpConstantTo(CurrentDoorLocation, InitDoorLocation, DeltaTime, DoorOpeningSpeed); // Flexible way to change Yaw
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Z = CurrentDoorLocation;
	
	GetOwner()->SetActorLocation(DoorLocation);

	if (!AudioComponent)
	{
		return;
	}
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = false;
		CloseDoorSound = true;
	}
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

	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}