// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UBlinkComponent::UBlinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();

	Mesh = static_cast<USkeletalMeshComponent*>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}


// Called every frame
void UBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float time = GetWorld()->GetTimeSeconds();
	if (bBlinking)
	{
		const float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
		if (time > EndTime || FMath::IsNearlyEqual(time, EndTime))
		{
			EndBlink();
		}
		else
		{
			float t = ElapsedTime / (EndTime - StartTime);
			t = BlinkCurve->GetFloatValue(t);
			FVector newLoc = FMath::Lerp(StartLocation, EndLocation, t);

			GetOwner()->SetActorLocation(newLoc, false, nullptr, ETeleportType::ResetPhysics);
		}
	}
}


void UBlinkComponent::BlinkToAbsolute(const FVector Location, float Duration, FBlinkLambdaCallback Callback)
{
	if (IsBlinking()) return;

	PrepBlink(Location, Duration, Callback);
}


void UBlinkComponent::BlinkInDirection(FVector BlinkDirection, float BlinkDistance, float Duration, FBlinkLambdaCallback Callback)
{
	if (IsBlinking()) return;

	BlinkDirection.Normalize(.1);
	BlinkDirection *= BlinkDistance;
	BlinkToRelative(BlinkDirection, Duration, Callback);
}


void UBlinkComponent::BlinkToRelative(FVector RelativeLocation, float Duration, FBlinkLambdaCallback Callback)
{
	if (IsBlinking()) return;

	const FVector ActorLocation = GetOwner()->GetActorLocation();
	if (RelativeLocation.IsNearlyZero(.1))
	{
		BlinkToAbsolute(ActorLocation, Duration, Callback);
		return;
	}

	BlinkToAbsolute(ActorLocation + RelativeLocation, Duration, Callback);
}


void UBlinkComponent::StartBlink(FBlinkCallback Callback)
{
	check(bBlinkPrepped);
	CallbackFromStart = Callback;
	bBlinking = true;
	StartLocation = GetOwner()->GetActorLocation();
	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + BlinkDuration;
	GetOwner()->SetActorEnableCollision(false);
}


void UBlinkComponent::PrepBlink(const FVector To, float Duration, FBlinkLambdaCallback Callback)
{
	bBlinkPrepped = true;
	EndLocation = To;
	BlinkDuration = Duration;
	CallbackFromPrep = Callback;
}


void UBlinkComponent::EndBlink()
{
	bBlinking = false;
	bBlinkPrepped = false;
	GetOwner()->SetActorLocation(EndLocation);
	GetOwner()->SetActorEnableCollision(true);
	
	CallbackFromPrep.ExecuteIfBound();
	CallbackFromStart.ExecuteIfBound();
}
