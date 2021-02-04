// Copyright Samuel Carriere 2021


#include "NPCController.h"
#include "../Pawns/GravityPawn.h"
#include "../Actors/Planet.h"
#include "../Actors/Projectile.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

ANPCController::ANPCController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCController::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = GetPawn()->GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(WanderTimeHandle, this, &ANPCController::Wander, 1.f, false);
}

void ANPCController::Wander()
{
	AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
	if (GravityPawn == nullptr) return;

	APlanet* Planet = Cast<APlanet>(GravityPawn->GetAttractor());
	if (Planet == nullptr)
	{
		TargetPosition = InitialPosition + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f));
		DrawDebugPoint(GetWorld(), TargetPosition, 8.f, FColor::Red, true);
		IsWandering = true;
	}
	else
	{
		TargetPosition = Planet->GetRandomPosition();
		DrawDebugPoint(GetWorld(), TargetPosition, 8.f, FColor::Red, true);
		IsWandering = true;
	}
}

void ANPCController::Tick(float DeltaTime)
{
	if (IsChasing)
	{
		TargetPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		if (UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), TargetPosition) > ShootingDistance)
		{
			//TODO currently moving before rotating as the pawn will move towards the ground
			FVector forwardVelocity = GetPawn()->GetActorForwardVector() * ForwardAcceleration;
			AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
			GravityPawn->GetMeshComponent()->SetPhysicsLinearVelocity(forwardVelocity, true);
		}
		else if (!IsShooting)
		{
			IsShooting = true;
			GetWorld()->GetTimerManager().SetTimer(ShootTimeHandle, this, &ANPCController::Shoot, 0.5f, false);
		}
		//TODO fix the rotation to be flat to the surface (currently pointing toward the ground)
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), TargetPosition);
		GetPawn()->SetActorRotation(LookAtRotation);
	}
	else if(IsWandering)
	{
		if (UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) < DetectionDistance)
		{
			TargetPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			IsChasing = true;
			IsWandering = false;
			GetWorld()->GetTimerManager().ClearTimer(WanderTimeHandle);
		}
		if (UKismetMathLibrary::Vector_Distance(GetPawn()->GetActorLocation(), TargetPosition) > 100.f)
		{
			//TODO currently moving before rotating as the pawn will move towards the ground
			FVector forwardVelocity = GetPawn()->GetActorForwardVector() * ForwardAcceleration;
			AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
			GravityPawn->GetMeshComponent()->SetPhysicsLinearVelocity(forwardVelocity, true);

			//TODO fix the rotation to be flat to the surface (currently pointing toward the ground)
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), TargetPosition);
			GetPawn()->SetActorRotation(LookAtRotation);
		}
		else
		{
			IsWandering = false;
			GetWorld()->GetTimerManager().SetTimer(WanderTimeHandle, this, &ANPCController::Wander, 5.f, false);
		}
	}
}

void ANPCController::Shoot()
{
	AGravityPawn* GravityPawn = Cast<AGravityPawn>(GetPawn());
	if (GravityPawn != nullptr)
	{
		GravityPawn->Shoot();
	}
	IsShooting = false;
}