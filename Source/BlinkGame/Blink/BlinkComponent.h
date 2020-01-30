// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "Delegates/Delegate.h"
#include "BlinkComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLINKGAME_API UBlinkComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* BlinkCurve;

public:	
	// Sets default values for this component's properties
	UBlinkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BlinkTo(FVector Location, float Duration);
	void BlinkInDirection(FVector BlinkDirection, float BlinkDistance, float Duration);
	void BlinkInDirection(FVector RelativeLocation, float Duration);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsBlinking() const { return bBlinking; }

private:
	void StartBlink(FVector From, FVector To, float Duration);
	void EndBlink();

	USkeletalMeshComponent* Mesh;
	FVector StartLocation;
	FVector EndLocation;
	bool bBlinking;
	float StartTime;
	float EndTime;
};
