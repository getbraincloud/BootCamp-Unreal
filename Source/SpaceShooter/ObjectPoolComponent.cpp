// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ObjectPoolComponent.h"
#include "PaperSprite.h"


// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UObjectPoolComponent::CreatePool(TSubclassOf<class ABaseActor> baseActorSubclass, int poolSize)
{
    if (baseActorSubclass != nullptr)
    {
        UWorld* const world = GetWorld();
        if (world != nullptr)
        {
            for (int i = 0; i < poolSize; i++)
            {
                ABaseActor* objectPoolActor = world->SpawnActor<ABaseActor>(baseActorSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
                objectPoolActor->Deactivate();
                m_ObjectPool.Add(objectPoolActor);
            }
        }
    }
}

ABaseActor * UObjectPoolComponent::SpawnActor()
{
    for (ABaseActor* a : m_ObjectPool)
        if (!a->IsActive())
            return a;

    return nullptr;
}

std::vector<ABaseActor*> UObjectPoolComponent::GetActiveObjects()
{
    std::vector<ABaseActor*> activeObjects;
    
    for (ABaseActor* a : m_ObjectPool)
        if (a->IsActive())
            activeObjects.push_back(a);
    
    return activeObjects;
}

void UObjectPoolComponent::DeactivateAll()
{
    for (ABaseActor* a : m_ObjectPool)
        a->Deactivate();
}

