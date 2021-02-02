// Copyright Samuel Carriere 2021


#include "ControllablePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AControllablePawn::AControllablePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraArm->TargetArmLength = 600.f;
	CameraArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;
	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
}

void AControllablePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AControllablePawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AControllablePawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AControllablePawn::LookRightRate);
}

void AControllablePawn::MoveForward(float AxisValue)
{
	FVector forwardVelocity = this->GetActorForwardVector() * AxisValue * ForwardAcceleration;
	MeshComponent->SetPhysicsLinearVelocity(forwardVelocity, true);
}

void AControllablePawn::MoveRight(float AxisValue)
{
	FVector rightVelocity = this->GetActorRightVector() * AxisValue * RightAcceleration;
	MeshComponent->SetPhysicsLinearVelocity(rightVelocity, true);
}

void AControllablePawn::LookRightRate(float AxisValue)
{
	this->AddActorLocalRotation(FRotator(0.f, AxisValue, 0.f));
}
