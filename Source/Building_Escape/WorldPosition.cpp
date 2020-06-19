// Fill out your copyright notice in the Description page of Project Settings.
// #define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-01, 1.4f, FColor::Green, text);
#include "WorldPosition.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG(LogTemp, Fatal, TEXT("This is a Fatal!"));
	UE_LOG(LogTemp, Error, TEXT("This is a Error!"));
	UE_LOG(LogTemp, Warning, TEXT("This is a Warning!"));
	UE_LOG(LogTemp, Display, TEXT("This is a Display!"));
	UE_LOG(LogTemp, Log, TEXT("This is a Log!"));

	// Log to Screen
	// print("HELLO GALAXY");	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

