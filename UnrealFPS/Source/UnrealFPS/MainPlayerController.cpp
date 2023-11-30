// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling() && bIsSliding)
	{
		GetWorldTimerManager().PauseTimer(SlideHandle);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(SlideHandle);
	}
}

// Called to bind functionality to input
void AMainPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Jumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Crouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMainPlayerController::Slide);
		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Sprinting);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Sprinting);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Shot);

	}

}

/// <summary>
/// Move the player in the direction of the input
/// if w is pressed move forward if s is pressed move backwards etc.
/// </summary>
/// <param name="value">Vector2D for X and Y</param>
void AMainPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("WASD pressed X: %f Y: %f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/// <summary>
/// move the camera in the direction of the mouse movement
/// if the mouse moves right move the camera right etc.
/// </summary>
/// <param name="value">vectoe2D from mouse movement</param>
void AMainPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/// <summary>
/// when the jump button is pressed check if the player is sliding
/// if the player is not sliding jump normally
/// if the player is sliding add a force to the z axis to make the player jump but make sure the player is on the ground
/// </summary>
void AMainPlayerController::Jumping()
{
	//check if the player is sliding
	if (!bIsSliding)
	{
		//if the player is not sliding jump
		Jump();
	}
	else
	{
		//check if the player is on the ground
		if(!GetCharacterMovement()->IsFalling())
		{
			//get the direction the player is moving
			FVector Direction = GetCharacterMovement()->Velocity.GetSafeNormal();
			Direction.Z = 1.0f;
			//add a force to the direction the player is moving
			GetCharacterMovement()->AddImpulse(Direction * 600.0f, true);
		}
	}
}

/// <summary>
/// if the crouch button is pressed crouch
/// if the crouch button is released stand up
/// </summary>
void AMainPlayerController::Crouching()
{
	if(bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
		UnCrouch();
		bIsCrouched = false;
	}
	else
	{
		Crouch();
		bIsCrouched = true;
	}
}

/// <summary>
/// if the sprint button is pressed sprint
/// if the sprint button is released stop sprinting
/// </summary>
void AMainPlayerController::Sprinting()
{
	if (bIsSprinting)
	{
		Slide();
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	}
}

/// <summary>
/// check if the player is moving fast enough to slide
/// if the player is moving fast enough slide by increasing the max walk speed
/// if the player is not moving fast enough stop sliding by decreasing the max walk speed
/// </summary>
void AMainPlayerController::Slide()
{
	if (GetCharacterMovement()->Velocity.Size() > 700.0f)
	{
		bIsSliding = true;
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 1200.0f;
		//after 2 seconds stop sliding
		GetWorldTimerManager().SetTimer(SlideHandle, this, &AMainPlayerController::StopSliding, 1.2f, false);
	}
	else
	{
		//cancel timer
		GetWorldTimerManager().ClearTimer(SlideHandle);
		bIsSliding = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	}

}

void AMainPlayerController::StopSliding()
{
	bIsSliding = false;
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 1200.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	}
	UnCrouch();
}

void AMainPlayerController::Shot()
{
	UWeapon* Weapon = Cast<UWeapon>(GetComponentByClass(UWeapon::StaticClass()));
	if (Weapon)
	{
		Weapon->Fire();
	}
}

