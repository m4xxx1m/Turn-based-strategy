// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Explosion.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API AProjectile : public AActor {
    GENERATED_BODY()

public:
    AProjectile();

    void Initialize(const UAbility *Ability,
                    uint8 playerIndex,
                    float PathLength);

    void Shoot(FVector From, FVector To) const;

protected:
    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AExplosion> ExplosionSubclass;

    // virtual void NotifyHit(UPrimitiveComponent *MyComp,
    //                        AActor *Other,
    //                        UPrimitiveComponent *OtherComp,
    //                        bool bSelfMoved,
    //                        FVector HitLocation,
    //                        FVector HitNormal,
    //                        FVector NormalImpulse,
    //                        const FHitResult &Hit) override;

    UPROPERTY(Replicated)
    float Damage;

    UPROPERTY(Replicated)
    int8 PlayerIndex = -1;

    UPROPERTY(Replicated)
    float SplashRadius;

    // UPROPERTY(EditAnywhere, Replicated)
    // USphereComponent *CollisionComponent;

    UPROPERTY(EditAnywhere, Replicated)
    UStaticMeshComponent *ProjectileMeshComponent;

    UPROPERTY(VisibleAnywhere, Replicated)
    UProjectileMovementComponent *ProjectileMovementComponent;

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(Server, Reliable)
    void Explode() const;
};
