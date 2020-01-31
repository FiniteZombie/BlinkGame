// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCharacter.h"
#include "Blink/BlinkComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

#pragma region Constructor

// Sets default values
AHeroCharacter::AHeroCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	BlinkDuration = 1.f;
	DashDistance = 500.f;
	JumpBlinkDistance = 500.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create blink component
	BlinkComponent = CreateDefaultSubobject<UBlinkComponent>(TEXT("BlinkComponent"));
}

#pragma endregion Constructor

#pragma region Input

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHeroCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &AHeroCharacter::Evade);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &AHeroCharacter::Attack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHeroCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHeroCharacter::LookUpAtRate);
}

void AHeroCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !BlinkComponent->IsBlinking())
	{
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !BlinkComponent->IsBlinking())
	{
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHeroCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHeroCharacter::Jump()
{
	Super::Jump();
	FBlinkLambdaCallback Callback;
	Callback.BindLambda([this] {});
	BlinkComponent->BlinkToRelative(FVector(0.f, 0.f, JumpBlinkDistance), BlinkDuration, Callback);
}

void AHeroCharacter::Evade()
{
	if (Controller != nullptr)
	{
		const float MoveForwardAxis = GetInputAxisValue("MoveForward");
		const float MoveRightAxis = GetInputAxisValue("MoveRight");

		if (FMath::Abs(MoveForwardAxis) < .1f && FMath::Abs(MoveRightAxis) < .1f)
			return; // Nothing for now
		
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// Get right vector
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		Forward *= MoveForwardAxis;
		Right *= MoveRightAxis;

		const FVector MoveInput = Forward + Right;
		FVector NormalDirection;
		float Length;
		MoveInput.ToDirectionAndLength(NormalDirection, Length);
		
		Length = FMath::Min(1.0f, Length);
		const FVector Direction = NormalDirection * Length * DashDistance;

		// Figure out which dash to play based on the character's rotation relative to dash direction
		const FVector ActorForward = GetActorForwardVector();
		const FVector ActorRight = GetActorRightVector();
		const float DotForward = FVector::DotProduct(ActorForward, NormalDirection);
		const float DotRight = FVector::DotProduct(ActorRight, NormalDirection);

		DirectionEnum DashDirection;

		if (FMath::Abs(DotForward) > FMath::Abs(DotRight))
		{
			DashDirection = DotForward > 0.f ? DirectionEnum::Fwd : DirectionEnum::Bck;
		}
		else
		{
			DashDirection = DotRight > 0.f ? DirectionEnum::Right : DirectionEnum::Left;
		}

		SetActorRotation(ActorForward.Rotation());
		PlayAnimMontage(DashMontage, 1.f, "FwdDashBegin");
		
		FBlinkLambdaCallback Callback;
		Callback.BindLambda([this, DashDirection]
			{
				PlayDashEnd(DashDirection);
			});
		BlinkComponent->BlinkToRelative(Direction, BlinkDuration, Callback);
	}
}

void AHeroCharacter::Attack()
{
	// Nothing for now
}

#pragma endregion Input

#pragma region Helpers

void AHeroCharacter::PlayDashEnd(DirectionEnum Direction)
{
	//PlayAnimMontage(DashMontage, 1.f, "LeftDashEnd");
	switch (Direction)
	{
	case Fwd:
		PlayAnimMontage(DashMontage, 1.f, "FwdDashEnd");
		break;
	case Bck:
		PlayAnimMontage(DashMontage, 1.f, "BckDashEnd");
		break;
	case Right:
		PlayAnimMontage(DashMontage, 1.f, "RightDashEnd");
		break;
	case Left:
		PlayAnimMontage(DashMontage, 1.f, "LeftDashEnd");
		break;
	}
}

#pragma endregion Helpers