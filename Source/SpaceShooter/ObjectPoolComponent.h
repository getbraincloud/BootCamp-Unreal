// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseActor.h"
#include <vector>
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESHOOTER_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

    // Creates the ObjectPool for the supplied subclass
    void CreatePool(TSubclassOf<class ABaseActor> baseActorSubclass, int poolSize);
    
    // Spawns an actor and returns a pointer to the object
    ABaseActor* SpawnActor();
    
    // Returns a std::vector of BaseActor pointer to all the active objects in the ObjectPool
    std::vector<ABaseActor*> GetActiveObjects();
    
    // Deactivates every object in the ObjectPool
    void DeactivateAll();
    
private:
    TArray<ABaseActor*> m_ObjectPool;
};
