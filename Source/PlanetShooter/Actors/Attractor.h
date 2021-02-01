// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractor.generated.h"

UCLASS()
class PLANETSHOOTER_API AAttractor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttractor();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
