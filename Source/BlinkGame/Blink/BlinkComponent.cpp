// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

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


void UBlinkComponent::BlinkTo(const FVector Location, float Duration)
{
	if (bBlinking) return;

	StartBlink(GetOwner()->GetActorLocation(), Location, Duration);
}


void UBlinkComponent::BlinkInDirection(FVector BlinkDirection, float BlinkDistance, float Duration)
{
	if (bBlinking) return;

	BlinkDirection.Normalize(.1);
	BlinkDirection *= BlinkDistance;
	BlinkInDirection(BlinkDirection, Duration);
}


void UBlinkComponent::BlinkInDirection(FVector RelativeLocation, float Duration)
{
	if (bBlinking) return;

	const FVector ActorLocation = GetOwner()->GetActorLocation();
	if (RelativeLocation.IsNearlyZero(.1))
	{
		BlinkTo(ActorLocation, Duration);
		return;
	}

	BlinkTo(ActorLocation + RelativeLocation, Duration);
}


void UBlinkComponent::StartBlink(const FVector From, const FVector To, float Duration)
{
	StartLocation = From;
	EndLocation = To;
	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + Duration;
	bBlinking = true;
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorHiddenInGame(true);
	Mesh->bPauseAnims = true;
}


void UBlinkComponent::EndBlink()
{
	GetOwner()->SetActorLocation(EndLocation);
	bBlinking = false;
	GetOwner()->SetActorEnableCollision(true);
	GetOwner()->SetActorHiddenInGame(false);
	Mesh->bPauseAnims = false;
}
