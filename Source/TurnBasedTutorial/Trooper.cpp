#include "Trooper.h"
#include <Kismet/GameplayStatics.h>

#include "HealthBar.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATrooper::ATrooper()
    : HitPoints(StartHitPoints), ActionPoints(StartActionPoints) {
    bReplicates = true;

    PrimaryActorTick.bCanEverTick = true;
    Tags.Add(FName("Trooper"));
    AttackAbility = CreateDefaultSubobject<UAbility>("AttackAbility");
    SpecialAbility = CreateDefaultSubobject<UAbility>("SpecialAbility");

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(
        "HealthBar");
    HealthWidgetComponent->AttachToComponent(RootComponent,
                                             FAttachmentTransformRules::KeepRelativeTransform);

    SelectionStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        "SelectionMesh");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT(
        "StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
    SelectionStaticMesh->AttachToComponent(RootComponent,
                                           FAttachmentTransformRules::KeepRelativeTransform);
    SelectionStaticMesh->SetRelativeScale3D({1.8, 1.8, 0.01});
    SelectionStaticMesh->SetVisibility(false);

    if (MeshToUse.Object) {
        SelectionStaticMesh->SetStaticMesh(MeshToUse.Object);
        SelectionStaticMesh->SetStaticMesh(MeshToUse.Object);
    }
    // SelectionStaticMesh->SetRelativeTransform(FTransform({1000,1000,100}, {0, 0, 0}), false,
    // nullptr, ETeleportType::TeleportPhysics);
    // SelectionStaticMesh->

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
    Cast<UHealthBar>(HealthWidgetComponent->GetUserWidgetObject())->
        SetOwnerTrooper(this);
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
        if (PositionVector.Size() >= (TargetLocation - CurrentLocation).
            Size()) {
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
    ActionPoints -= (NewPos - CurrentLocation).Size() * MoveCost;
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
    DOREPLIFETIME(ATrooper, HitPoints);
    DOREPLIFETIME(ATrooper, ActionPoints);
    DOREPLIFETIME(ATrooper, HealthWidgetComponent);
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


float ATrooper::GetActionRadius(int action) const {
    switch (action) {
        case 1:
            return AttackAbility->ActionRadius;
        case 2:
            return SpecialAbility->ActionRadius;
        default:
            return ActionPoints;
    }
}

float ATrooper::GetHitPoints() const {
    return HitPoints;
}

float ATrooper::GetMaxHitPoints() const {
    return StartHitPoints;
}

void ATrooper::SetSelection(bool Selection) const {
    if (SelectionStaticMesh) {
        if (SelectionStaticMesh->GetMaterial(0) != GreenMaterial) {
            SelectionStaticMesh->SetMaterial(0, GreenMaterial);
        }
        SelectionStaticMesh->SetVisibility(Selection);
    }
}

void ATrooper::HighlightAsEnemy() const {
    SelectionStaticMesh->SetVisibility(true);
}

void ATrooper::ResetActionPoints() {
    ActionPoints = StartActionPoints;
}

UAbility *ATrooper::GetAbility(int AbilityIndex) const {
    switch (AbilityIndex) {
        case 1:
            return AttackAbility;
        case 2:
            return SpecialAbility;
        default:
            return nullptr;
    }
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

void ATrooper::Attack(int abilityIndex) {
    bIsAttacking = true;
    ActionPoints -= GetAbility(abilityIndex)->ActionCost;
}

bool ATrooper::CheckMoveCorrectness(const FVector newPos) const {
    return (newPos - CurrentLocation).Size() * MoveCost <= ActionPoints;
    // return (newPos - CurrentLocation).Size() <= MoveRadius;
}

bool ATrooper::CheckAttackCorrectness(const FVector attackLocation,
                                      int abilityIndex) const {
    return (attackLocation - CurrentLocation).Size() <=
           GetAbility(abilityIndex)->ActionRadius && ActionPoints >=
           GetAbility(abilityIndex)->ActionCost;
    // return (attackLocation - CurrentLocation).Size() <= AttackRadius;
}
