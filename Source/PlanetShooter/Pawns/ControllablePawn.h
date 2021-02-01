// // Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "GravityPawn.h"
#include "ControllablePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PLANETSHOOTER_API AControllablePawn : public AGravityPawn
{
	GENERATED_BODY()
	
public:
	AControllablePawn();

private:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float ForwardAcceleration = 10.f;

	UPROPERTY(EditAnywhere)
	float RightAcceleration = 10.f;

	UPROPERTY(EditAnywhere)
	float JumpAcceleration = 850.f;

protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRightRate(float AxisValue);
	void Jump();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
