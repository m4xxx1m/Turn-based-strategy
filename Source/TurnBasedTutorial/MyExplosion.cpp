// Fill out your copyright notice in the Description page of Project Settings.


#include "MyExplosion.h"

#include "Trooper.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

AMyExplosion::AMyExplosion() {
    // if (!CollisionComponent) {
    //     CollisionComponent = CreateDefaultSubobject<USphereComponent>(
    //         TEXT("SphereComponent"));
    //     RootComponent = CollisionComponent;
    // }
    if (!ParticleSystemComponent) {
        ParticleSystemComponent = CreateDefaultSubobject<
            UParticleSystemComponent>(
            TEXT("ParticleSystemComponent"));
        RootComponent = ParticleSystemComponent;
    }
    InitialLifeSpan = 1.0f;
}

void AMyExplosion::Initialize(float damage,
                              float splashRadius,
                              uint8 playerIndex) {
    Damage = damage;
    PlayerIndex = playerIndex;
    float Scale = splashRadius / 50;
    // CollisionComponent->SetWorldScale3D({Scale, Scale, Scale});
    if (ParticleSystemComponent && ParticleSystemComponent->IsValidLowLevel()) {
        ParticleSystemComponent->SetWorldScale3D({Scale, Scale, Scale});
    }
}

void AMyExplosion::BeginPlay() {
    Super::BeginPlay();
}

void AMyExplosion::NotifyActorBeginOverlap(AActor *OtherActor) {
    Super::NotifyActorBeginOverlap(OtherActor);
    ATrooper *OtherTrooper = Cast<ATrooper>(OtherActor);
    if (OtherTrooper) {
        UE_LOG(LogTemp, Warning,
               TEXT(
                   "Begin explosion overlap: id: %d, index: %d, damage: %f, my index: %d"
               ),
               OtherTrooper->GetId(), OtherTrooper->GetPlayerIndex(), Damage,
               PlayerIndex);
        if (PlayerIndex != -1 && PlayerIndex != OtherTrooper->
            GetPlayerIndex()) {
            OtherTrooper->TrooperTakeDamage(Damage);
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Overlapped not a trooper"));
    }
}

void AMyExplosion::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyExplosion, Damage);
    DOREPLIFETIME(AMyExplosion, PlayerIndex);
    // DOREPLIFETIME(AMyExplosion, CollisionComponent);
    DOREPLIFETIME(AMyExplosion, ParticleSystemComponent);
}
