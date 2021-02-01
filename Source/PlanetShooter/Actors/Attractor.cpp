// Copyright Samuel Carriere 2021


#include "Attractor.h"

AAttractor::AAttractor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AAttractor::BeginPlay()
{
	Super::BeginPlay();
}

void AAttractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
