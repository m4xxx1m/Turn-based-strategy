// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadTrooper.h"

AManageSquadTrooper::AManageSquadTrooper() {
    GetMesh()->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AManageSquadTrooper::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AManageSquadTrooper::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AManageSquadTrooper::
ChangeSkeletalMesh(const AManageSquadTrooper *OtherTrooper) const {
    GetMesh()->SetSkeletalMesh(OtherTrooper->GetMesh()->SkeletalMesh);
}

ETrooperType AManageSquadTrooper::GetType() const {
    return Type;
}

int AManageSquadTrooper::GetIndex() const {
    return Index;
}

void AManageSquadTrooper::Initialize(int TrooperIndex) {
    Index = TrooperIndex;
    Type = ETrooperType::TROOPER_IN_SQUAD;
}
