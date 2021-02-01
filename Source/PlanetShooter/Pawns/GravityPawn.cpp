// Copyright Samuel Carriere 2021


#include "GravityPawn.h"
#include "../Actors/Attractor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGravityPawn::AGravityPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetLinearDamping(.3f);
	MeshComponent->SetAngularDamping(500.f);
	RootComponent = MeshComponent;
}

void AGravityPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAttractor::StaticClass(), FoundActors);
	Attractor = Cast<AAttractor>(FoundActors[0]);
}

void AGravityPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector gravityDirection = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(Attractor->GetActorLocation(), this->GetActorLocation()));
	FRotator gravityRotation = UKismetMathLibrary::MakeRotFromZX(gravityDirection, this->GetActorForwardVector());
	this->SetActorRotation(gravityRotation);

	FVector gravityVelocity = gravityDirection * -GravityAcceleration;
	MeshComponent->SetPhysicsLinearVelocity(gravityVelocity, true);
}
