// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameMode/BlinkGameGameModeBase.h"
#include "Engine/Engine.h"

UWorld* UDrawDebugLibrary::GetWorldFromActor(AActor* Actor)
{
	return Actor->GetWorld();
}

UWorld* UDrawDebugLibrary::GetWorldFromComponent(UActorComponent* Component)
{
	return Component->GetWorld();
}

void UDrawDebugLibrary::Print(UWorld* World, int Key, FString Text, float duration = 0.f)
{
	if (IsDrawDebugDisabled(World)) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(Key, duration, FColor::White, Text);
}

void UDrawDebugLibrary::DrawSector(
	UWorld* World, 
	const FVector Center, 
	const FVector Direction, 
	const float Radius,
	const float Angle, 
	const int Segments, 
	const FLinearColor Color,
	const float LifeTime)
{
	if (IsDrawDebugDisabled(World)) return;

	TArray<FVector> Points = TArray<FVector>();
	const int NumOuterPoints = 1 + Segments;
	Points.Reserve(NumOuterPoints);

	FVector StartVector = Radius * Direction.GetUnsafeNormal();
	const float HalfAngle = Angle / 2.f;
	StartVector = StartVector.RotateAngleAxis(-HalfAngle, FVector::UpVector);

	const float SubSectorAngle = Angle / Segments;
	FVector CurrentVector = StartVector;
	FVector PrevVector = StartVector;
	for (int i = 0; i < NumOuterPoints; i++)
	{
		if (i > 0)
		{
			CurrentVector = StartVector.RotateAngleAxis(i * SubSectorAngle, FVector::UpVector);
			DrawLine(World, Center + PrevVector, Center + CurrentVector, Color, LifeTime);
		}

		if (i == 0 || i == NumOuterPoints - 1)
			DrawLine(World, Center, Center + CurrentVector, Color, LifeTime);

		PrevVector = CurrentVector;
	}
}

void UDrawDebugLibrary::DrawCircleUp(
	UWorld* World, 
	const FVector Center, 
	const int Segments, 
	const FLinearColor Color,
	const float LifeTime)
{
	if (IsDrawDebugDisabled(World)) return;

	static const float Radius = 100.f;
	static const FVector YAxis = FVector(0, 1, 0);
	static const FVector ZAxis = FVector(-1, 0, 0);

	DrawDebugCircle(World, Center, Radius, Segments, Color.ToFColor(false), bPersistentLines, LifeTime, DepthPriority, Thickness, YAxis, ZAxis);
}

void UDrawDebugLibrary::DrawCircleSide(
	UWorld* World, 
	const FVector Center, 
	const int Segments, 
	const FLinearColor Color,
	const FVector Direction,
	const float LifeTime)
{
	if (IsDrawDebugDisabled(World)) return;

	static const float Radius = 100.f;
	static const FVector YAxis = FVector::CrossProduct(Direction, FVector::UpVector);
	static const FVector ZAxis = FVector(0, 0, 1);

	DrawDebugCircle(World, Center, Radius, Segments, Color.ToFColor(false), bPersistentLines, LifeTime, DepthPriority, Thickness, YAxis, ZAxis);
}

void UDrawDebugLibrary::DrawLine(UWorld* World, const FVector LineStart, const FVector LineEnd, const FLinearColor Color, const float LifeTime)
{
	if (IsDrawDebugDisabled(World)) return;

	DrawDebugLine(World, LineStart, LineEnd, Color.ToFColor(false), bPersistentLines, LifeTime, DepthPriority, Thickness);
}

void UDrawDebugLibrary::DrawArrow(UWorld* World, const FVector LineStart, const FVector LineEnd, const float ArrowSize, const FLinearColor Color, const float LifeTime)
{
	if (IsDrawDebugDisabled(World)) return;

	DrawDebugDirectionalArrow(World, LineStart, LineEnd, ArrowSize, Color.ToFColor(false), bPersistentLines, LifeTime, DepthPriority, Thickness);
}

bool UDrawDebugLibrary::IsDrawDebugDisabled(UWorld* World)
{
	ABlinkGameGameModeBase* GameMode = Cast<ABlinkGameGameModeBase>(World->GetAuthGameMode());
	return GameMode != nullptr && !GameMode->bShowDrawDebug;
}
