// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboInterface.h"
#include "QuickAttackCombo.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLINKGAME_API UQuickAttackCombo : public UActorComponent, public IComboInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Timing, meta = (AllowPrivateAccess = "true"))
	float InputInterval;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Timing, meta = (AllowPrivateAccess = "true"))
	float IntervalShrinkRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Anim, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* Montage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TArray<FName> SectionNames;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool bWaitingForAnim;

public:	
	// Sets default values for this component's properties
	UQuickAttackCombo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Combo)
	void Play();

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSection(int NewSectionOverride);

private:
	void Attack();

	float LastInputTimeStamp;
	float LastAttackTimeStamp;
	float NextAttackTime;
	class ACharacter* Character;
	int CurrentSection;
	int SectionOverride;
};
