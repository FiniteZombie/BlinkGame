// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"

// Sets default values for this component's properties
UCombatManager::UCombatManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatManager::RegisterPlayer(ACharacter* Character)
{
	Player = Character;
}

void UCombatManager::RegisterEnemy(ACharacter* Character)
{
	Enemies.Add(Character);
}