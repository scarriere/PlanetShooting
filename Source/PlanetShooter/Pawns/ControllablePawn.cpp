// Copyright Samuel Carriere 2021


#include "ControllablePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Actors/Planet.h"

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

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AControllablePawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AControllablePawn::Shoot);
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

void AControllablePawn::Jump()
{
	FVector upVelocity = this->GetActorUpVector() * JumpAcceleration;
	MeshComponent->SetPhysicsLinearVelocity(upVelocity, true);
}

void AControllablePawn::Shoot()
{
	const FName TraceTag("Shoot_Trace");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(TEXT("Shoot_Trace"), false, this);
	FVector PawnHeight = GetActorLocation() + GetActorUpVector() * 50.f;
	if (GetWorld()->LineTraceSingleByChannel(Hit, PawnHeight, PawnHeight + GetActorForwardVector() * Reach, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		APlanet* Planet = Cast<APlanet>(Hit.Actor.Get());
		if (Planet != NULL)
		{
			Planet->ResourceHit(Hit.GetComponent(), Hit.Item);
		}
	}
}