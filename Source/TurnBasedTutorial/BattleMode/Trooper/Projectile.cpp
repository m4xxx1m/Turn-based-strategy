// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Trooper.h"
#include "Net/UnrealNetwork.h"

AProjectile::AProjectile() {
    // if (!RootComponent) {
    //     RootComponent = CreateDefaultSubobject<USceneComponent>(
    //         TEXT("ProjectileSceneComponent"));
    // }
    // if (!CollisionComponent) {
    //     CollisionComponent = CreateDefaultSubobject<USphereComponent>(
    //         TEXT("SphereComponent"));
    //     RootComponent = CollisionComponent;
    // }
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
            // CollisionComponent);
            ProjectileMeshComponent);
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->InitialSpeed = 1000.0f;
        ProjectileMovementComponent->MaxSpeed = 1000.0f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }
    InitialLifeSpan = 2.0f;
}

void AProjectile::Initialize(const UAbility *Ability,
                             uint8 playerIndex,
                             float PathLength) {
    ProjectileMovementComponent->InitialSpeed =
        ProjectileMovementComponent->MaxSpeed = Ability->Speed;
    Damage = Ability->Damage;
    SplashRadius = Ability->SplashRadius;
    float Scale = Ability->LinearWidth / 100;
    // CollisionComponent->SetSphereRadius(Ability->LinearWidth / 2);
    ProjectileMeshComponent->SetWorldScale3D({Scale, Scale, Scale});
    PlayerIndex = playerIndex;
    SetLifeSpan(PathLength / Ability->Speed);
}

void AProjectile::Shoot(FVector From, FVector To) const {
    ProjectileMovementComponent->Velocity =
        (To - From).GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::NotifyActorBeginOverlap(AActor *OtherActor) {
    Super::NotifyActorBeginOverlap(OtherActor);
    ATrooper *OtherTrooper = Cast<ATrooper>(OtherActor);
    if (OtherTrooper) {
        UE_LOG(LogTemp, Warning,
               TEXT("Begin overlap: id: %d, index: %d, damage: %f, my index: %d"
               ),
               OtherTrooper->GetId(), OtherTrooper->GetPlayerIndex(), Damage,
               PlayerIndex);
        if (Damage > 0) {
            if (PlayerIndex != -1 && PlayerIndex != OtherTrooper->
                GetPlayerIndex()) {
                OtherTrooper->TrooperTakeDamage(Damage);
            }
        } else {
            if (PlayerIndex != -1 && PlayerIndex == OtherTrooper->
                GetPlayerIndex()) {
                OtherTrooper->TrooperTakeDamage(Damage);
            }
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Overlapped not a trooper"));
    }
}

// void AMyProjectile::NotifyHit(UPrimitiveComponent *MyComp,
//                               AActor *Other,
//                               UPrimitiveComponent *OtherComp,
//                               bool bSelfMoved,
//                               FVector HitLocation,
//                               FVector HitNormal,
//                               FVector NormalImpulse,
//                               const FHitResult &Hit) {
//     Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation,
//                      HitNormal,
//                      NormalImpulse, Hit);
//     ATrooper *OtherTrooper = Cast<ATrooper>(Other);
//     if (OtherTrooper) {
//         UE_LOG(LogTemp, Warning,
//                TEXT("On Hit: id: %d, index: %d, damage: %f, my index: %d"
//                ),
//                OtherTrooper->GetId(), OtherTrooper->GetPlayerIndex(), Damage,
//                PlayerIndex);
//         if (PlayerIndex != -1 && PlayerIndex != OtherTrooper->
//             GetPlayerIndex()) {
//             OtherTrooper->TakeDamage(Damage);
//         }
//     } else {
//         UE_LOG(LogTemp, Warning, TEXT("Overlapped not a trooper"));
//     }
// }


void AProjectile::BeginPlay() {
    Super::BeginPlay();
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    Explode();
}

void AProjectile::Explode_Implementation() const {
    const FTransform SpawnTransform = GetTransform();
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetInstigator();
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AExplosion *Explosion = GetWorld()->SpawnActor<AExplosion>(
        ExplosionSubclass, SpawnTransform, SpawnParameters);
    Explosion->Initialize(Damage, SplashRadius, PlayerIndex);
    Explosion->SetActorLocation(GetActorLocation());
}

void AProjectile::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AProjectile, Damage);
    DOREPLIFETIME(AProjectile, PlayerIndex);
    // DOREPLIFETIME(AMyProjectile, CollisionComponent);
    DOREPLIFETIME(AProjectile, ProjectileMeshComponent);
    DOREPLIFETIME(AProjectile, ProjectileMovementComponent);
    DOREPLIFETIME(AProjectile, SplashRadius);
}
