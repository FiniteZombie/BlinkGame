// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "Delegates/Delegate.h"
#include "BlinkComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FBlinkCallback);

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

	UFUNCTION(BlueprintCallable)
	void BlinkToAbsolute(FVector Location, float Duration);

	UFUNCTION(BlueprintCallable)
	void BlinkInDirection(FVector BlinkDirection, float BlinkDistance, float Duration);

	UFUNCTION(BlueprintCallable)
	void BlinkToRelative(FVector RelativeLocation, float Duration);

	UFUNCTION(BlueprintCallable)
	void StartBlink(FBlinkCallback Callback);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsBlinking() const { return bBlinking || bBlinkPrepped; }

private:
	void PrepBlink(FVector To, float Duration);
	void EndBlink();

	USkeletalMeshComponent* Mesh;
	FVector StartLocation;
	FVector EndLocation;
	bool bBlinking;
	bool bBlinkPrepped;
	float StartTime;
	float EndTime;
	FBlinkCallback BlinkCallback;
	float BlinkDuration;
};
