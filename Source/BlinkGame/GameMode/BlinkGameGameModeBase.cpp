// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "BlinkGameGameModeBase.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"

void ABlinkGameGameModeBase::ToggleDrawDebug()
{
	bShowDrawDebug = !bShowDrawDebug;
}

void ABlinkGameGameModeBase::SetTimeDilation(float Ratio) const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	AWorldSettings* Settings = World->GetWorldSettings();
	Settings->SetTimeDilation(Ratio);
}
