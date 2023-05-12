// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyProjectile.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API AMyProjectile : public AActor {
    GENERATED_BODY()

public:
    AMyProjectile();

    void Initialize(const UAbility *Ability, uint8 playerIndex, float PathLength);

    void Shoot(FVector From, FVector To) const;

protected:
    UPROPERTY(Replicated)
    float Damage;

    UPROPERTY(Replicated)
    uint8 PlayerIndex = -1;
    
    // UPROPERTY(EditAnywhere)
    // USphereComponent *CollisionComponent;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent *ProjectileMeshComponent;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent *ProjectileMovementComponent;
    
    virtual void BeginPlay() override;
};
