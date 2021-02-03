// Copyright Samuel Carriere 2021


#include "HumanPawn.h"
#include "../Actors/Planet.h"
#include "ShipPawn.h"

void AHumanPawn::Jump()
{
	FVector upVelocity = this->GetActorUpVector() * JumpAcceleration;
	MeshComponent->SetPhysicsLinearVelocity(upVelocity, true);
}

void AHumanPawn::Shoot()
{
	const FName TraceTag("Shoot_Trace");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(TEXT("Shoot_Trace"), false, this);
	FVector PawnHeight = GetActorLocation() + GetActorUpVector() * 50.f;
	if (GetWorld()->LineTraceSingleByChannel(Hit, PawnHeight, PawnHeight + GetActorForwardVector() * Reach, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		// Refactor using interface and no cast
		APlanet* Planet = Cast<APlanet>(Hit.Actor.Get());
		if (Planet)
		{
			Planet->ResourceHit(Hit.GetComponent(), Hit.Item);
			return;
		}
		AShipPawn* Ship = Cast<AShipPawn>(Hit.Actor.Get());
		if (Ship)
		{
			Ship->Board(this);
			return;
		}
	}
}

void AHumanPawn::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHumanPawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AHumanPawn::Shoot);
}
