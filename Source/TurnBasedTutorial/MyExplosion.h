// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyExplosion.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API AMyExplosion : public AActor {
    GENERATED_BODY()

public:
    AMyExplosion();

    void Initialize(float damage,
                    float splashRadius,
                    uint8 playerIndex);

protected:
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

    UPROPERTY(Replicated)
    float Damage;

    UPROPERTY(Replicated)
    int8 PlayerIndex = -1;
    
    // UPROPERTY(EditAnywhere, Replicated)
    // USphereComponent *CollisionComponent;

    UPROPERTY(EditAnywhere, Replicated)
    UParticleSystemComponent *ParticleSystemComponent;
};
