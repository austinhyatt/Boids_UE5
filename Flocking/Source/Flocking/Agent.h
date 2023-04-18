#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class FLOCKING_API AAgent : public AActor
{
	GENERATED_BODY()

public:
	AAgent();
	void Init(UStaticMeshComponent* mesh, int id);
	UStaticMeshComponent* Mesh;
	FVector Velocity;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
