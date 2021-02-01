// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GravityPawn.generated.h"

class AAttractor;

UCLASS()
class PLANETSHOOTER_API AGravityPawn : public APawn
{
	GENERATED_BODY()

public:
	AGravityPawn();

private:
	UPROPERTY(EditAnywhere)
	AAttractor* Attractor;

	UPROPERTY(EditAnywhere)
	float GravityAcceleration = 10.f;

	UPROPERTY(EditAnywhere)
	float MaxVelocity = 100.f;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
