#include "FlockingManager.h"

#define AGENT_COUNT 10

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));

    World = world;
    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    FVector v1, v2, v3, v4;
    for (int i = 0; i < AGENT_COUNT; i++) {
        v1 = Rule1(Agents[i]);
        v2 = Rule2(Agents[i]);
        v3 = Rule3(Agents[i]);
        v4 = Boundary(Agents[i]);

        Agents[i]->Velocity += v1 + v2 + v3 + v4;

        LimitVelocity(Agents[i]);
    }
}

//average position
FVector UFlockingManager::Rule1(AAgent* Agent) {
    FVector averagePos = FVector();
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] == Agent)
            continue;
        averagePos += Agents[i]->GetActorLocation();
    }
    averagePos /= AGENT_COUNT - 1;
    return (averagePos - Agent->GetActorLocation()) / 100; //scale
    
}

//maintain distance
FVector UFlockingManager::Rule2(AAgent* Agent) {
    FVector flockProximity = FVector();
    
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] == Agent)
            continue;
        if ((Agents[i]->GetActorLocation() - Agent->GetActorLocation()).GetAbs().Size() < 100) //dist
            flockProximity -= (Agents[i]->GetActorLocation() - Agent->GetActorLocation());
    }
    
    return flockProximity;
}

//average velocity
FVector UFlockingManager::Rule3(AAgent* Agent) {
    FVector averageVelocity = FVector();
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] == Agent)
            continue;
        averageVelocity += Agents[i]->Velocity;
    }
    averageVelocity /= AGENT_COUNT - 1;
    return (averageVelocity - Agent->Velocity) / 8; //speed
}

void UFlockingManager::LimitVelocity(AAgent* Agent) {
    int vlim = 50; //max speed
    if ((Agent->Velocity).GetAbs().Size() > vlim)
        Agent->Velocity = (Agent->Velocity / (Agent->Velocity).GetAbs().Size()) * vlim;
}

FVector UFlockingManager::Boundary(AAgent* Agent) {
        
    int boxSize = 1000; //box size
    int Xmin=-boxSize, Xmax=boxSize, Ymin=-boxSize, Ymax= boxSize, Zmin=0, Zmax= boxSize*2;
    int boundForce = 10;
    FVector boundVelocity = FVector();

    if (Agent->GetActorLocation().X < Xmin)
        boundVelocity.X = boundForce;
    else if (Agent->GetActorLocation().X > Xmax)
        boundVelocity.X = -boundForce;
    if (Agent->GetActorLocation().Y < Ymin)
        boundVelocity.Y = boundForce;
    else if (Agent->GetActorLocation().Y > Ymax)
        boundVelocity.Y = -boundForce;
    if (Agent->GetActorLocation().Z < Zmin)
        boundVelocity.Z = boundForce;
    else if (Agent->GetActorLocation().Z > Zmax)
        boundVelocity.Z = -boundForce;
    
    return boundVelocity;
}

