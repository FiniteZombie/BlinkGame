// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "Delegates/Delegate.h"
#include "BlinkComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FBlinkCallback);
DECLARE_DELEGATE(FBlinkLambdaCallback);

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

	void BlinkToAbsolute(FVector Location, float Duration, FBlinkLambdaCallback Callback);
	void BlinkInDirection(FVector BlinkDirection, float BlinkDistance, float Duration, FBlinkLambdaCallback Callback);
	void BlinkToRelative(FVector RelativeLocation, float Duration, FBlinkLambdaCallback Callback);

	UFUNCTION(BlueprintCallable)
	void StartBlink(FBlinkCallback Callback);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsBlinking() const { return bBlinking || bBlinkPrepped; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector LastBlinkDirection() const { return EndLocation - StartLocation; }

private:
	void PrepBlink(FVector To, float Duration, FBlinkLambdaCallback Callback);
	void EndBlink();

	USkeletalMeshComponent* Mesh;
	FVector StartLocation;
	FVector EndLocation;
	bool bBlinking;
	bool bBlinkPrepped;
	float StartTime;
	float EndTime;
	FBlinkLambdaCallback CallbackFromPrep;
	FBlinkCallback CallbackFromStart;
	float BlinkDuration;
};
