#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

#define DebugPrint(key, text) if (GEngine) GEngine->AddOnScreenDebugMessage(key, 0, FColor::White,text)
#define DebugCircleUp(Center, Segments, Color) DrawDebugCircle(GetWorld(), Center, 100.f, Segments, Color, false, -1, 0, 0, FVector(0, 1, 0), FVector(-1, 0, 0))
#define DebugCircleSide(Center, Segments, Color, Direction) DrawDebugCircle(GetWorld(), Center, 100.f, Segments, Color, false, -1, 0, 0, FVector::CrossProduct(Direction, FVector::UpVector), FVector(0, 0, 1))

class DebugUtil
{
public:
	static void DrawDebugSector(UWorld* World, const FVector Center, const FVector Direction, const float Radius, const float Angle, const int Segments, const FColor Color)
	{
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
				DrawDebugLine(World, Center + PrevVector, Center + CurrentVector, Color);
			}

			if (i == 0 || i == NumOuterPoints - 1)
				DrawDebugLine(World, Center, Center + CurrentVector, Color);
			
			PrevVector = CurrentVector;
		}
	}
};