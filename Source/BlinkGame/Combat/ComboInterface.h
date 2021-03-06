// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UComboInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLINKGAME_API IComboInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Combo)
	void Play();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Anim)
	bool GetAnimWait() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Anim)
	void SetAnimWait(bool NewAnimWait);
};
