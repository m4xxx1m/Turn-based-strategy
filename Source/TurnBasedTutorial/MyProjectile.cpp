// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"

#include "Net/UnrealNetwork.h"
#include "VisualLogger/VisualLoggerCustomVersion.h"

AMyProjectile::AMyProjectile() {
    // if (!CollisionComponent) {
    //     CollisionComponent = CreateDefaultSubobject<USphereComponent>(
    //         TEXT("SphereComponent"));
    // }
    // RootComponent = CollisionComponent;
    if (!ProjectileMeshComponent) {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(
            TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(
            TEXT(
                "StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
        if (Mesh.Succeeded()) {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
            RootComponent = ProjectileMeshComponent;
        }
    }
    if (!ProjectileMovementComponent) {
        ProjectileMovementComponent = CreateDefaultSubobject<
            UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(
            ProjectileMeshComponent);
        ProjectileMovementComponent->InitialSpeed = 1000.0f;
        ProjectileMovementComponent->MaxSpeed = 1000.0f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }
    InitialLifeSpan = 2.0f;
}

void AMyProjectile::Initialize(const UAbility *Ability,
                               uint8 playerIndex,
                               float PathLength) {
    ProjectileMovementComponent->InitialSpeed =
        ProjectileMovementComponent->MaxSpeed = Ability->Speed;
    Damage = Ability->Damage;
    float Scale = Ability->LinearWidth / 100;
    ProjectileMeshComponent->SetWorldScale3D({Scale, Scale, Scale});
    PlayerIndex = playerIndex;
    SetLifeSpan(PathLength / Ability->Speed);
}

void AMyProjectile::Shoot(FVector From, FVector To) const {
    ProjectileMovementComponent->Velocity =
        (To - From).GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
}


void AMyProjectile::BeginPlay() {
    Super::BeginPlay();
}

void AMyProjectile::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyProjectile, Damage);
}
