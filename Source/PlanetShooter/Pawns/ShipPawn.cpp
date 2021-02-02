// Copyright Samuel Carriere 2021


#include "ShipPawn.h"
#include "HumanPawn.h"

void AShipPawn::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	AControllablePawn::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AShipPawn::Interact);
}

void AShipPawn::Interact()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = GetActorLocation() + GetActorUpVector() * 100.f;
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(PassengerClass, SpawnLocation, GetActorRotation(), SpawnInfo);
	GetWorld()->GetFirstPlayerController()->Possess(Spawned);
}

void AShipPawn::Board(AActor* Passenger)
{
	GetWorld()->GetFirstPlayerController()->Possess(this);
	PassengerClass = Passenger->GetClass();
	Passenger->Destroy();
}