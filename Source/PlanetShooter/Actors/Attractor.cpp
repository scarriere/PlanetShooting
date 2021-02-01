// Copyright Samuel Carriere 2021


#include "Attractor.h"

AAttractor::AAttractor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAttractor::BeginPlay()
{
	Super::BeginPlay();
}

void AAttractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
