// Copyright Samuel Carriere 2021

#pragma once

#include "CoreMinimal.h"
#include "ControllablePawn.h"
#include "HumanPawn.generated.h"

UCLASS()
class PLANETSHOOTER_API AHumanPawn : public AControllablePawn
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	float JumpAcceleration = 850.f;

	UPROPERTY(EditAnywhere)
	float Reach = 250.f;

protected:
	void Jump();
	void Collect();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
