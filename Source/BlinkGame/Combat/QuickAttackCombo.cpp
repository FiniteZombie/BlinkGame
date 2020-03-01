// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAttackCombo.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UQuickAttackCombo::UQuickAttackCombo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	// ...
}

void UQuickAttackCombo::SetSection(int NewSectionOverride)
{
	if (NewSectionOverride < SectionNames.Num())
		SectionOverride = NewSectionOverride;
}

void UQuickAttackCombo::Play_Implementation()
{
	if (Character == nullptr)
		return;

	if (CurrentSection >= SectionNames.Num() || CurrentSection < 0)
		CurrentSection = 0;

	const FName SectionName = SectionNames[CurrentSection];
	Character->PlayAnimMontage(Montage, 1.f, SectionName);
	CurrentSection++;
}

