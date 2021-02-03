// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

UCLASS()
class PLANETSHOOTER_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCController();

private:
	UPROPERTY(EditAnywhere)
	FVector TargetPosition;

	UPROPERTY(EditAnywhere)
	float ForwardAcceleration = 10.f;

	FTimerHandle WanderTimeHandle;

	bool IsWandering = false;
	void Wander();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
