// Copyright Samuel Carriere 2021

#include "Planet.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RandomStream.h"

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
		MeshComponent->SetRelativeScale3D(FVector(PlanetScale, PlanetScale, PlanetScale));

		InstanceMeshComponent->ClearInstances();
		PlanetSeed.Reset();
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
			if (InstanceMeshComponent != NULL && PlanetSeed.RandHelper(2) == 1)
			{
				FRotator InstanceRotation = UKismetMathLibrary::MakeRotFromZ(FaceCenter / 3);
				FTransform InstanceTransform = FTransform(InstanceRotation, FaceCenter / 3 * PlanetScale);
				InstanceMeshComponent->AddInstance(InstanceTransform);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Planet Instance count %d"), InstanceMeshComponent->GetInstanceCount())
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
