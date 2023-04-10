#include "Trooper.h"
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"

// Sets default values
ATrooper::ATrooper() {
    bReplicates = true;

    PrimaryActorTick.bCanEverTick = true;
    Tags.Add(FName("Trooper"));
    MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    RootComponent = MyStaticMesh;
    MeshPath = TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(MeshPath);
    if (MeshToUse.Object) {
        MyStaticMesh->SetStaticMesh(MeshToUse.Object);
    }
}

// void ATrooper::SetStaticMesh() const {
//     static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(MeshPath);
//     if (MeshToUse.Object) {
//         MyStaticMesh->SetStaticMesh(MeshToUse.Object);
//     }
// }

// Called when the game starts or when spawned
void ATrooper::BeginPlay() {
    Super::BeginPlay();
}

void ATrooper::Initialize(uint8 const NewPlayerIndex,
                          FVector const SpawnLocation,
                          uint8 const NewId) {
    PlayerIndex = NewPlayerIndex;
    bIsMoving = false;
    CurrentLocation = SpawnLocation;
    Id = NewId;
}

void ATrooper::Tick(float const DeltaTime) {
    if (!bIsMoving)
        return;
    FVector PositionVector = (TargetLocation - CurrentLocation);
    PositionVector.Normalize();
    PositionVector *= (Speed * DeltaTime);
    if (PositionVector.Size() >= (TargetLocation - CurrentLocation).Size()) {
        CurrentLocation = TargetLocation;
        bIsMoving = false;
    } else {
        CurrentLocation += PositionVector;
    }
    SetActorLocation(CurrentLocation);
}

void ATrooper::MoveTrooper(FVector const NewPos) {
    TargetLocation = NewPos;
    bIsMoving = true;
}

uint8 ATrooper::GetId() const {
    return Id;
}


void ATrooper::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    DOREPLIFETIME(ATrooper, PlayerIndex);
    DOREPLIFETIME(ATrooper, CurrentLocation);
    DOREPLIFETIME(ATrooper, TargetLocation);
    DOREPLIFETIME(ATrooper, bIsMoving);
    DOREPLIFETIME(ATrooper, Id);
}

uint8 ATrooper::GetPlayerIndex() const {
    return PlayerIndex;
}

//
// ATrooperWizard::ATrooperWizard() {
//     MeshPath = TEXT(
//         // "StaticMesh'/Game/CityofBrass_Enemies/Static/Wizard_StaticMesh.Wizard_StaticMesh'");
//         "StaticMesh'/Game/CityofBrass_Enemies/Static/SkeletonMelee_StaticMesh.SkeletonMelee_StaticMesh'");
//     SetStaticMesh();
// }
//
// ATrooperSkeletonMelee::ATrooperSkeletonMelee() {
//     MeshPath = TEXT(
//         "StaticMesh'/Game/CityofBrass_Enemies/Static/SkeletonMelee_StaticMesh.SkeletonMelee_StaticMesh'");
//     SetStaticMesh();
// }
