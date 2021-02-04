// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "ControllablePawn.h"
#include "ShipPawn.generated.h"

class AProjectile;

UCLASS()
class PLANETSHOOTER_API AShipPawn : public AControllablePawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PassengerClass;

	UPROPERTY(EditAnywhere)
	float PitchSpeed = 40.f;

	UPROPERTY(EditAnywhere)
	float RollSpeed = 60.f;

	UPROPERTY(EditAnywhere)
	float TrusterAcceleration = 30.f;

	UPROPERTY(EditAnywhere)
	float LaunchAcceleration = 300.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	bool TrusterOn = false;
	bool Launching = false;

protected:
	virtual void MoveForward(float AxisValue) override;
	virtual void MoveRight(float AxisValue) override;
	void Interact();
	void ToggleTruster();
	void InitiateLaunch();
	void Shoot();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	void Board(AActor* Passenger);
};
