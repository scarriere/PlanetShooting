// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

class AProjectile;

UCLASS()
class PLANETSHOOTER_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCController();

private:
	UPROPERTY(EditAnywhere)
	FVector TargetPosition;

	UPROPERTY(VisibleAnywhere)
	FVector InitialPosition;

	UPROPERTY(EditAnywhere)
	float ForwardAcceleration = 10.f;

	UPROPERTY(EditAnywhere)
	float DetectionDistance = 500.f;

	UPROPERTY(EditAnywhere)
	float ShootingDistance = 500.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle WanderTimeHandle;
	FTimerHandle ShootTimeHandle;

	bool IsWandering = false;
	bool IsChasing = false;
	bool IsShooting = false;
	void Wander();
	void Shoot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
