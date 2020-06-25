// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// Logging out to test
	// UE_LOG(LogTemp, Warning,
	// 	   TEXT("PlayerViewPointLocation: %s, PlayerViewPointRotation: %s"),
	// 	   *PlayerViewPointLocation.ToString(),
	// 	   *PlayerViewPointRotation.ToString());

	// Draw a line for showing the reach
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector();
	// LineTraceEnd *= FVector(0.f, 0.f, DebugLineReach);
	// LineTraceEnd *= DebugLineReach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor::Purple,
		false,
		0.f,
		0,
		5.f);

	// GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation,	PlayerViewPointRotation);
	// Ray-cast out to a certain distance (Reach)

	// See what it hits
}
