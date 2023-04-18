#pragma once

#include "Agent.h"
#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

		void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;
	FVector Rule1(AAgent* boid);
	FVector Rule2(AAgent* boid);
	FVector Rule3(AAgent* boid);
	FVector Boundary(AAgent* boid);
	void LimitVelocity(AAgent* boid);
};
