// Copyright Samuel Carriere 2021


#include "ShipPawn.h"
#include "HumanPawn.h"
#include "Misc/App.h"

void AShipPawn::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AShipPawn::Interact);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShipPawn::ToggleTruster);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AShipPawn::ToggleTruster);
	PlayerInputComponent->BindAction(TEXT("Launch"), IE_Released, this, &AShipPawn::InitiateLaunch);
}

void AShipPawn::Interact()
{
	if (Attractor != nullptr)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = GetActorLocation() + GetActorUpVector() * 100.f;
		APawn* Spawned = GetWorld()->SpawnActor<APawn>(PassengerClass, SpawnLocation, GetActorRotation(), SpawnInfo);
		GetWorld()->GetFirstPlayerController()->Possess(Spawned);
	}
}

void AShipPawn::Board(AActor* Passenger)
{
	GetWorld()->GetFirstPlayerController()->Possess(this);
	PassengerClass = Passenger->GetClass();
	Passenger->Destroy();
}

void AShipPawn::ToggleTruster()
{
	TrusterOn = !TrusterOn;
}

void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Launching)
	{
		FVector upVelocity = this->GetActorUpVector() * LaunchAcceleration;
		MeshComponent->SetPhysicsLinearVelocity(upVelocity, true);
		if (Attractor == nullptr)
		{
			Launching = false;
		}
	}

	if (TrusterOn)
	{
		FVector forwardVelocity = this->GetActorForwardVector() * TrusterAcceleration;
		MeshComponent->SetPhysicsLinearVelocity(forwardVelocity, true);
	}
}

void AShipPawn::MoveForward(float AxisValue)
{
	if (Attractor == nullptr)
	{
		this->AddActorLocalRotation(FRotator(-AxisValue * GetWorld()->GetDeltaSeconds() * PitchSpeed, 0.f, 0.f));
	}
	else
	{
		Super::MoveForward(AxisValue);
	}
}

void AShipPawn::MoveRight(float AxisValue)
{
	if (Attractor == nullptr)
	{
		this->AddActorLocalRotation(FRotator(0.f, 0.f, AxisValue * GetWorld()->GetDeltaSeconds() * RollSpeed));
	}
	else
	{
		Super::MoveRight(AxisValue);
	}
}

void AShipPawn::InitiateLaunch()
{
	if (Attractor != nullptr && !Launching)
	{
		Launching = true;
	}
}