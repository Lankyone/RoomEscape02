// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
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
	FindPhysicsComponent();
	SetupInputComponent();
	
	
}
void UGrabber::FindPhysicsComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle found!"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handler component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component found!"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
	}
}



void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed!"))
		GetFirstPhysicsBodyInReach();
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"))
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;


	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
	return FHitResult();
}