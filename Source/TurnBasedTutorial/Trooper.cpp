#include "Trooper.h"
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"

// Sets default values
ATrooper::ATrooper() {
    bReplicates = true;

    PrimaryActorTick.bCanEverTick = true;
    Tags.Add(FName("Trooper"));
    // MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    // RootComponent = MyStaticMesh;
    // MeshPath = TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'");
    // static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(MeshPath);
    // if (MeshToUse.Object) {
    //     MyStaticMesh->SetStaticMesh(MeshToUse.Object);
    // }
}

// void ATrooper::SetStaticMesh() const {
//     static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(
//         TEXT(
//             "StaticMesh'/Game/CityofBrass_Enemies/Static/Wizard_StaticMesh.Wizard_StaticMesh'"
//         )
//     );
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
    AttackPlayedTime = 0.0f;
    CurrentLocation = SpawnLocation;
    Id = NewId;
}

void ATrooper::Tick(float const DeltaTime) {
    if (bIsAttacking) {
        AttackPlayedTime += DeltaTime;
        if (AttackPlayedTime >= AttackDuration) {
            AttackPlayedTime = 0.0f;
            bIsAttacking = false;
        }
    }
    if (bIsMoving) {
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
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATrooper, PlayerIndex);
    DOREPLIFETIME(ATrooper, CurrentLocation);
    DOREPLIFETIME(ATrooper, TargetLocation);
    DOREPLIFETIME(ATrooper, bIsMoving);
    DOREPLIFETIME(ATrooper, Id);
    DOREPLIFETIME(ATrooper, bIsAttacking);
    DOREPLIFETIME(ATrooper, AttackPlayedTime);
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

FVector ATrooper::GetLocation() const {
    return CurrentLocation;
}

void ATrooper::Attack() {
    bIsAttacking = true;
}

float ATrooper::GetAnimationValue() {
    if (bIsAttacking) {
        return -100.0f;
    }
    if (bIsMoving) {
        return 100.0f;
    }
    return 0.0f;
}

bool ATrooper::CheckMoveCorrectness(const FVector newPos) const {
    return (newPos - CurrentLocation).Size() <= MoveRadius;
}

bool ATrooper::CheckAttackCorrectness(const FVector attackLocation) const {
    return (attackLocation - CurrentLocation).Size() <= AttackRadius;
}
