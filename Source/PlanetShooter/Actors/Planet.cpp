// Copyright Samuel Carriere 2021

#include "Planet.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RandomStream.h"
#include "Math/UnrealMathUtility.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = true;

	InstanceMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	InstanceMeshComponent->SetupAttachment(RootComponent);
}


void APlanet::OnConstruction(const FTransform & Transform)
{
	if (MeshComponent != NULL && MeshComponent->GetStaticMesh() != NULL)
	{
		InstanceMeshComponent->ClearInstances();
		PlanetSeed.Reset();
		PlanetMap.Empty();

		MeshComponent->SetRelativeScale3D(FVector(PlanetScale, PlanetScale, PlanetScale));
		UStaticMesh* Mesh = MeshComponent->GetStaticMesh();
		FIndexArrayView IndexBuffer = Mesh->RenderData->LODResources[0].IndexBuffer.GetArrayView();

		for (int32 FaceId = 0; FaceId < Mesh->RenderData->LODResources[0].GetNumTriangles(); ++FaceId)
		{
			FVector FaceCenter = FVector::ZeroVector;
			for (int32 VertexId = 0; VertexId < 3; ++VertexId)
			{
				uint32 VertexIndex = IndexBuffer[FaceId * 3 + VertexId];
				FVector VertexPos = Mesh->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex);
				FaceCenter += VertexPos;
			}
			FaceCenter = FaceCenter / 3;
			PlanetMap.Add(FaceCenter * PlanetScale);
			if (InstanceMeshComponent != NULL && PlanetSeed.RandHelper(2) == 1)
			{
				FRotator InstanceRotation = UKismetMathLibrary::MakeRotFromZ(FaceCenter);
				FTransform InstanceTransform = FTransform(InstanceRotation, FaceCenter * PlanetScale);
				InstanceMeshComponent->AddInstance(InstanceTransform);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Planet Map Size %d"), PlanetMap.Num())
	}
}

void APlanet::ResourceHit(UPrimitiveComponent* ComponentHit, int32 ItemId)
{
	UInstancedStaticMeshComponent* InstanceComponentHit = Cast<UInstancedStaticMeshComponent>(ComponentHit);
	if (InstanceComponentHit != NULL)
	{
		InstanceComponentHit->RemoveInstance(ItemId);
	}
}

FVector APlanet::GetRandomPosition()
{
	if (PlanetMap.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Empty PlanetMap error!"))
		return FVector::ZeroVector;
	}
	return PlanetMap[FMath::RandHelper(PlanetMap.Num())];
}