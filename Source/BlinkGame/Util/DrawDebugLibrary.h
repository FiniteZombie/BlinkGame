// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "DrawDebugLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BLINKGAME_API UDrawDebugLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Debug")
	static UWorld* GetWorldFromActor(AActor* Actor);
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	static UWorld* GetWorldFromComponent(UActorComponent* Component);
	
	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void Print(UWorld* World, int Key, FString Text);

	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void DrawSector(
		UWorld* World,
		const FVector Center,
		const FVector Direction,
		const float Radius,
		const float Angle,
		const int Segments,
		const FLinearColor Color,
		const float LifeTime = -1);

	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void DrawCircleUp(
		UWorld* World,
		const FVector Center,
		const int Segments,
		const FLinearColor Color,
		const float LifeTime = -1);

	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void DrawCircleSide(
		UWorld* World,
		const FVector Center,
		const int Segments,
		const FLinearColor Color,
		const FVector Direction,
		const float LifeTime = -1);

	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void DrawLine(
		UWorld* World,
		const FVector LineStart,
		const FVector LineEnd,
		const FLinearColor Color,
		const float LifeTime = -1);

	UFUNCTION(BlueprintCallable, Category = "DrawDebug")
	static void DrawArrow(
		UWorld* World, 
		FVector LineStart, 
		FVector LineEnd, 
		float ArrowSize, 
		FLinearColor Color, 
		float LifeTime = -1);

private:
	static bool IsDrawDebugDisabled(UWorld* World);
	
	static const bool bPersistentLines = false;
	static const int DepthPriority = 0;
	static const int Thickness = 2;
};
