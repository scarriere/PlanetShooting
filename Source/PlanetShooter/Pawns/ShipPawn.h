// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "ControllablePawn.h"
#include "ShipPawn.generated.h"

UCLASS()
class PLANETSHOOTER_API AShipPawn : public AControllablePawn
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PassengerClass;

protected:
	void Interact();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Board(AActor* Passenger);
};
