// Copyright Samuel Carriere 2021


#include "GravityPawn.h"
#include "../Actors/Attractor.h"
#include "../Actors/Projectile.h"
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

AAttractor* AGravityPawn::GetAttractor()
{
	return Attractor;
}

UStaticMeshComponent* AGravityPawn::GetMeshComponent()
{
	return MeshComponent;
}

void AGravityPawn::BeginPlay()
{
	Super::BeginPlay();

	FindAttractor();
}

void AGravityPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attractor != nullptr)
	{
		FVector gravityDirection = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(Attractor->GetActorLocation(), this->GetActorLocation()));
		FRotator gravityRotation = UKismetMathLibrary::MakeRotFromZX(gravityDirection, this->GetActorForwardVector());
		this->SetActorRotation(gravityRotation);

		FVector gravityVelocity = gravityDirection * -GravityAcceleration;
		MeshComponent->SetPhysicsLinearVelocity(gravityVelocity, true);

		float distance = UKismetMathLibrary::Vector_Distance(Attractor->GetActorLocation(), this->GetActorLocation());
		if (distance > 2000.f)
		{
			Attractor = nullptr;
		}
	}
	else
	{
		FindAttractor();
	}

	// Need to fix the velocity to only use xy relative to the actor (gravity speed is separated)
	FVector currentVelocity = MeshComponent->GetComponentVelocity();
	FVector xyVelocity = FVector(currentVelocity.X, currentVelocity.Y, currentVelocity.Z);
	if (xyVelocity.Size() > MaxVelocity)
	{
		FVector clampVelocity = UKismetMathLibrary::ClampVectorSize(xyVelocity, 0.f, MaxVelocity);
		MeshComponent->SetPhysicsLinearVelocity(FVector(clampVelocity.X, clampVelocity.Y, clampVelocity.Z));
	}
}

void AGravityPawn::FindAttractor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAttractor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), this->GetActorLocation()) < 2000.f)
		{
			Attractor = Cast<AAttractor>(Actor);
		}
	}
}

void AGravityPawn::Shoot()
{
	FTransform ProjectileTransform(GetActorRotation(), GetActorLocation() + GetActorForwardVector() * ShootOffsetForward + GetActorUpVector() * ShootOffsetUp);
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass.Get(), ProjectileTransform);
}