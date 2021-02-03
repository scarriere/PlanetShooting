// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "Attractor.h"
#include "Planet.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class PLANETSHOOTER_API APlanet : public AAttractor
{
	GENERATED_BODY()

public:
	APlanet();

private:
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* InstanceMeshComponent;

	UPROPERTY(EditAnywhere)
	float PlanetScale = 5.f;

	UPROPERTY(EditAnywhere)
	FRandomStream PlanetSeed;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> PlanetMap;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	void ResourceHit(UPrimitiveComponent* ComponentHit, int32 ItemId);

	FVector GetRandomPosition();
};
