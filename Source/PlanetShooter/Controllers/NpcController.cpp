// Copyright Samuel Carriere 2021


#include "NPCController.h"
#include "../Pawns/GravityPawn.h"
#include "../Actors/Planet.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

ANPCController::ANPCController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(WanderTimeHandle, this, &ANPCController::Wander, 1.f, false);
}

void ANPCController::Wander()
{
	AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
	if (GravityPawn == nullptr) return;

	APlanet* Planet = Cast<APlanet>(GravityPawn->GetAttractor());
	if (Planet == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attractor undefined"))
		return;
	}

	TargetPosition = Planet->GetRandomPosition();
	DrawDebugPoint(GetWorld(), TargetPosition, 8.f, FColor::Red, true);
	IsWandering = true;
}

void ANPCController::Tick(float DeltaTime)
{
	if (IsWandering)
	{
		//HACK for testing follow
		TargetPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//TODO currently moving before rotating as the pawn will move towards the ground
		FVector forwardVelocity = GetPawn()->GetActorForwardVector() * ForwardAcceleration;
		AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
		GravityPawn->GetMeshComponent()->SetPhysicsLinearVelocity(forwardVelocity, true);

		//TODO fix the rotation to be flat to the surface (currently pointing toward the ground)
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), TargetPosition);
		GetPawn()->SetActorRotation(LookAtRotation);

		if (UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), TargetPosition) < 100.f)
		{
			//HACK for testing follow
			//IsWandering = false;
		}
	}
}