// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAttackCombo.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include <string>

// Sets default values for this component's properties
UQuickAttackCombo::UQuickAttackCombo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InputInterval = 1.f;
	IntervalShrinkRate = 0.95f;
	NextAttackTime = FLT_MAX;
}


// Called when the game starts
void UQuickAttackCombo::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UQuickAttackCombo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeSinceLastInput = CurrentTime - LastInputTimeStamp;

	if (NextAttackTime < CurrentTime && TimeSinceLastInput < InputInterval)
		Attack();
}

void UQuickAttackCombo::Play_Implementation()
{
	LastInputTimeStamp = GetWorld()->GetTimeSeconds();
	Attack();
}

void UQuickAttackCombo::CancelAnimWait_Implementation()
{
	bWaitingForAnim = false;
}

void UQuickAttackCombo::SetSection(int NewSectionOverride)
{
	if (NewSectionOverride < SectionNames.Num())
		SectionOverride = NewSectionOverride;
}

void UQuickAttackCombo::Attack()
{
	if (Character == nullptr)
		return;

	if (bWaitingForAnim)
		return;

	if (CurrentSection >= SectionNames.Num() || CurrentSection < 0)
		CurrentSection = 0;

	const FName SectionName = SectionNames[CurrentSection];
	Character->PlayAnimMontage(Montage, 1.f, SectionName);
	CurrentSection++;

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	float Interval = CurrentTime - LastAttackTimeStamp;
	Interval *= IntervalShrinkRate;
	NextAttackTime = CurrentTime + Interval;
	
	LastAttackTimeStamp = CurrentTime;
	bWaitingForAnim = true;
}

