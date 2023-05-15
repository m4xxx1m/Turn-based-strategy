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

    void Initialize(const UAbility *Ability,
                    uint8 playerIndex,
                    float PathLength);

    void Shoot(FVector From, FVector To) const;

protected:
    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

    virtual void NotifyHit(UPrimitiveComponent *MyComp,
                           AActor *Other,
                           UPrimitiveComponent *OtherComp,
                           bool bSelfMoved,
                           FVector HitLocation,
                           FVector HitNormal,
                           FVector NormalImpulse,
                           const FHitResult &Hit) override;

    UPROPERTY(Replicated)
    float Damage;

    UPROPERTY(Replicated)
    int8 PlayerIndex = -1;

    // UPROPERTY(EditAnywhere, Replicated)
    // USphereComponent *CollisionComponent;

    UPROPERTY(EditAnywhere, Replicated)
    UStaticMeshComponent *ProjectileMeshComponent;

    UPROPERTY(VisibleAnywhere, Replicated)
    UProjectileMovementComponent *ProjectileMovementComponent;

    virtual void BeginPlay() override;
};
