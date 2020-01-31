// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blink/BlinkComponent.h"
#include "HeroCharacter.generated.h"

UCLASS()
class BLINKGAME_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	float BlinkDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	float JumpBlinkDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	UAnimMontage* DashMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Blink)
	class UBlinkComponent* BlinkComponent;

	// Sets default values for this character's properties
	AHeroCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsBlinking() const { return BlinkComponent != nullptr && BlinkComponent->IsBlinking(); }

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Jump() override;
	void Evade();
	void Attack();

private:
	enum DirectionEnum { Fwd, Bck, Right, Left };
	void PlayDashEnd(DirectionEnum Direction);
};
