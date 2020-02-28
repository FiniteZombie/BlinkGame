// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlinkGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLINKGAME_API ABlinkGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug)
	bool bShowDrawDebug;

	UFUNCTION(Exec, Category = ExecFunctions)
	void ToggleDrawDebug();

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetTimeDilation(float Ratio) const;
};
