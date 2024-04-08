// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
struct FInputActionValue;
UCLASS()
class ELEMENTALFPS_API APlayerCharacter : public ACharacter
{
	FTimerHandle timer;
	int timesPlayed = 0;
	DECLARE_DELEGATE(FFireDelegate);
	FFireDelegate fire;

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess))
	UInputAction* moveAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SpawnActor();

	//Allows edit anywhere
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> actorBPToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	/***********GUN FIRING OPTIONS***********/
	UFUNCTION()
	void BasicFire() const;
	UFUNCTION()
	void EarthFire();
	
	void EarthFireTimerFunction();
	/***********END GUN FIRING OPTIONS*******/
	
	//Fire trigger to trigger the delegate
	void FireTrigger();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	
};
