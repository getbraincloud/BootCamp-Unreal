// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "ScrollingBackground.generated.h"

UCLASS()
class SPACESHOOTER_API AScrollingBackground : public AActor
{
	GENERATED_BODY()
	    
public:	
	// Sets default values for this actor's properties
	AScrollingBackground();
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components) USceneComponent* m_Root;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Actor Visual") class UPaperSpriteComponent* m_BackgroundSegment1;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Actor Visual") class UPaperSpriteComponent* m_BackgroundSegment2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    void MoveSegment(UPaperSpriteComponent* backgroundSegment, float displacement);
};
