// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "CombatManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLINKGAME_API UCombatManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatManager();
	
	UFUNCTION(BlueprintCallable)
	void RegisterPlayer(ACharacter* Character);

	UFUNCTION(BlueprintCallable)
	void RegisterEnemy(ACharacter* Character);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	TArray<ACharacter*> GetEnemies() const
	{
		return Enemies;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	ACharacter* Player;
	TArray<ACharacter*> Enemies;
};
