// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <string>
#include <string>

#include "Projectile.h"
#include "ElementalFPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Projects.h"
#include "Components/CapsuleComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
    fire.BindUObject(this, &APlayerCharacter::EarthFire);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d movementVec = Value.Get<FVector2d>();
	//Must do it like this as the other method just kinda goes NOPE Honestly had no idea why the other method worked anywho
	AddMovementInput(GetActorForwardVector(), movementVec.Y);
	AddMovementInput(GetActorRightVector(), movementVec.X);
	
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2d LookAxisVector = Value.Get<FVector2d>();
	
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::BasicFire() const
{
	FVector projectileSpawn = GetActorLocation();
	projectileSpawn.Z += 60.0f;
	//AProjectile* ProjectileObject = (AProjectile*) GetWorld()->SpawnActor<AProjectile>(projectileSpawn, GetFirstPersonCameraComponent()->GetComponentRotation());
	FRotator rotationSpawn = GetFirstPersonCameraComponent()->GetComponentRotation();
	GetWorld()->SpawnActor(ProjectileBP,&projectileSpawn, &rotationSpawn);

}

void APlayerCharacter::EarthFire()
{
		GetWorldTimerManager().SetTimer(timer, this, &APlayerCharacter::EarthFireTimerFunction, 0.2f, true, 0.0f);
}

void APlayerCharacter::EarthFireTimerFunction() 
{
	timesPlayed++;
	if(timesPlayed <= 5)
	{
		BasicFire();
	}
	else timesPlayed = 0, GetWorldTimerManager().ClearTimer(timer);
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//Fire Action
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireTrigger);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void APlayerCharacter::FireTrigger()
{
	if(fire.IsBound())
	{
		fire.Execute();
	}
	else GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Delegate no bound, Cannot fire weapon"));
}


//Spawn Code
void APlayerCharacter::SpawnActor()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(actorBPToSpawn, GetActorTransform(), spawnParams);
}


