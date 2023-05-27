#include "Trooper.h"
#include <Kismet/GameplayStatics.h>

#include "HealthBar.h"
#include "MyGameState.h"
#include "MyProjectile.h"
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
    SelectionStaticMesh->SetWorldScale3D({2.0f, 2.0f, 0.01f});
    SelectionStaticMesh->SetVisibility(false);

    if (MeshToUse.Object) {
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
    TakingDamagePlayedTime = 0.0f;
    CurrentLocation = SpawnLocation;
    Id = NewId;
}

void ATrooper::Tick(float const DeltaTime) {
    if (bIsAttacking) {
        AttackPlayedTime += DeltaTime;
        if (bIsWaitingForFire && AttackPlayedTime >= FireAfterTime) {
            FireProjectile();
            CurrentAbilityIndex = -1;
            CurrentAbilityDestination = {};
            bIsWaitingForFire = false;
        }
        if (AttackPlayedTime >= AttackDuration) {
            AttackPlayedTime = 0.0f;
            // bIsAttacking = false;
            SetIsAttacking(false);
        }
    }
    if (bIsTakingDamage) {
        TakingDamagePlayedTime += DeltaTime;
        if (TakingDamagePlayedTime >= TakingDamageDuration) {
            TakingDamagePlayedTime = 0.0f;
            // bIsTakingDamage = false;
            SetIsTakingDamage(false);
        }
    }
    if (bIsMoving) {
        FVector PositionVector = (TargetLocation - CurrentLocation);
        PositionVector.Normalize();
        PositionVector *= (Speed * DeltaTime);
        if (PositionVector.Size() >= (TargetLocation - CurrentLocation).
            Size()) {
            CurrentLocation = TargetLocation;
            // bIsMoving = false;
            SetIsMoving(false);
        } else {
            CurrentLocation += PositionVector;
        }
        SetActorLocation(CurrentLocation);
    }
}

void ATrooper::SetIsAttacking(bool IsAttacking) {
    bIsAttacking = IsAttacking;
    if (IsAttacking) {
        SetActorTickEnabled(true);
    } else {
        TryDisableTick();
    }
}

void ATrooper::SetIsTakingDamage(bool IsTakingDamage) {
    bIsTakingDamage = IsTakingDamage;
    if (IsTakingDamage) {
        SetActorTickEnabled(true);
    } else {
        TryDisableTick();
    }
}

void ATrooper::SetIsMoving(bool IsMoving) {
    bIsMoving = IsMoving;
    if (IsMoving) {
        SetActorTickEnabled(true);
    } else {
        TryDisableTick();
    }
}

void ATrooper::TryDisableTick() {
    if (!bIsAttacking && !bIsTakingDamage && !bIsMoving) {
        SetActorTickEnabled(false);
        if (AIController && AIController->IsValidLowLevel()) {
            AIController->ActionDone();
        }
    }
}

// void ATrooper::OnRepNotify_PlayerIndex() const {
//     const AMyPlayerState *player = Cast<AMyPlayerState>(
//         GetPlayerState());
//     if (!player)
//         return;
//     const uint8 ClientIndex = player->GetPlayerIndex();
//     UE_LOG(LogTemp, Warning,
//            TEXT("On rep notify, index: %d, client index: %d, id: %d"),
//            PlayerIndex,
//            ClientIndex, Id);
//     if (ClientIndex == PlayerIndex) {
//         HighlightAsEnemy();
//     }
// }

void ATrooper::MoveTrooper_Implementation(FVector const NewPos) {
    TargetLocation = NewPos;
    // bIsMoving = true;
    SetIsMoving(true);
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
    DOREPLIFETIME(ATrooper, TakingDamagePlayedTime);
    DOREPLIFETIME(ATrooper, bIsTakingDamage);
    DOREPLIFETIME(ATrooper, bIsDead);
    DOREPLIFETIME(ATrooper, CurrentAbilityIndex);
    DOREPLIFETIME(ATrooper, CurrentAbilityDestination);
}

int8 ATrooper::GetPlayerIndex() const {
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
            return AttackAbility->ActionCost <= ActionPoints
                       ? AttackAbility->ActionRadius
                       : 0;
        case 2:
            return SpecialAbility->ActionCost <= ActionPoints
                       ? SpecialAbility->ActionRadius
                       : 0;
        default:
            return ActionPoints / MoveCost;
    }
}

float ATrooper::GetRealActionRadius(int action) const {
    switch (action) {
        case 1:
            return AttackAbility->ActionRadius;
        case 2:
            return SpecialAbility->ActionRadius;
        default:
            return ActionPoints / MoveCost;
    }
}

float ATrooper::GetHitPoints() const {
    return HitPoints;
}

float ATrooper::GetMaxHitPoints() const {
    return StartHitPoints;
}

void ATrooper::SetSelection(bool Selection, uint8 ActionType) const {
    if (SelectionStaticMesh) {
        if (SelectionStaticMesh->GetMaterial(0) != GreenMaterial) {
            SelectionStaticMesh->SetMaterial(0, GreenMaterial);
            SelectionStaticMesh->SetRelativeLocation({0, 0, 1});
        }
        if (Selection) {
            UpdateSelectionRadius(ActionType);
        } else {
            SelectionStaticMesh->SetWorldScale3D({2.f, 2.f, 0.01f});
        }
        SelectionStaticMesh->SetVisibility(Selection);
    }
}

void ATrooper::UpdateSelectionRadius(uint8 ActionType) const {
    const float radiusScale =
        GetActionRadius(ActionType) / PIXELS_IN_RADIUS;
    SelectionStaticMesh->SetWorldScale3D(
        {radiusScale, radiusScale, 0.01f});
}

void ATrooper::HighlightAsEnemy_Implementation(int8 Index) const {
    if (PlayerIndex != Index) {
        SelectionStaticMesh->SetVisibility(true);
    }
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

void ATrooper::SetAIPossession(AEnemyAIController *EnemyController) {
    AIController = EnemyController;
}

bool ATrooper::IsDead() const {
    return bIsDead;
}

void ATrooper::TrooperTakeDamage_Implementation(float Damage) {
    if (bIsTakingDamage || bIsDead) {
        return;
    }
    HitPoints = FMath::Max<float>(0, HitPoints - Damage);
    if (HitPoints == 0) {
        bIsDead = true;
        SetLifeSpan(DyingAnimationDuration);
        GetWorld()->GetGameState<AMyGameState>()->DecreaseLivingTroopers(PlayerIndex);
    } else {
        // bIsTakingDamage = true;
        SetIsTakingDamage(true);
    }
}

TSubclassOf<AMyProjectile> ATrooper::GetProjectileClass(
    uint8 AbilityIndex) const {
    switch (AbilityIndex) {
        case 1:
            return AttackProjectileClass;
        case 2:
            return SpecialProjectileClass;
        default:
            return AMyProjectile::StaticClass();
    }
}

void ATrooper::FireProjectile_Implementation() {
    if (!this || !this->IsValidLowLevel())
        return;
    FTransform SpawnTransform(
        (CurrentAbilityDestination - CurrentLocation).Rotation());
    const FVector TransformedVector = SpawnTransform.TransformVector(
        {GetAbility(CurrentAbilityIndex)->LinearWidth / 2 + 50.0f, 0.0f,
         88.0f});
    const FVector SpawnLocation = CurrentLocation + TransformedVector;
    SpawnTransform.SetLocation(SpawnLocation);
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = this;
    SpawnParameters.Instigator = GetInstigator();
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AMyProjectile *Projectile = GetWorld()->SpawnActor<AMyProjectile>(
        GetProjectileClass(CurrentAbilityIndex), SpawnTransform,
        SpawnParameters);
    Projectile->SetActorLocation(SpawnLocation);
    Projectile->Initialize(GetAbility(CurrentAbilityIndex), PlayerIndex,
                           (CurrentAbilityDestination - SpawnLocation).
                           Size());
    Projectile->Shoot(SpawnLocation, CurrentAbilityDestination);
}

int ATrooper::GetAnimationValue() {
    if (bIsDead) {
        return 4;
    }
    if (bIsTakingDamage) {
        return 3;
    }
    if (bIsAttacking) {
        return 2;
    }
    if (bIsMoving) {
        return 1;
    }
    return 0;
}

void ATrooper::Attack_Implementation(int AbilityIndex, FVector ToLocation) {
    // bIsAttacking = true;
    SetIsAttacking(true);
    bIsWaitingForFire = true;
    ActionPoints -= GetAbility(AbilityIndex)->ActionCost;
    CurrentAbilityIndex = AbilityIndex;
    ToLocation.Z = 88.0f;
    CurrentAbilityDestination = ToLocation;
    // FTransform SpawnTransform((ToLocation - CurrentLocation).Rotation());
    // SpawnTransform.SetLocation(
    //     CurrentLocation + SpawnTransform.TransformVector(
    //         {GetAbility(AbilityIndex)->LinearWidth/2 + 50.0f, 0.0f, 0.0f})
    // );
    // FActorSpawnParameters SpawnParameters;
    // SpawnParameters.Owner = this;
    // SpawnParameters.Instigator = GetInstigator();
    // SpawnParameters.SpawnCollisionHandlingOverride =
    //     ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    // AMyProjectile *Projectile = GetWorld()->SpawnActor<AMyProjectile>(
    //     GetProjectileClass(AbilityIndex), SpawnTransform, SpawnParameters);
    // Projectile->Initialize(GetAbility(AbilityIndex));
    // Projectile->Shoot(CurrentLocation, ToLocation);
}

bool ATrooper::CheckMoveCorrectness(const FVector newPos) const {
    return (newPos - CurrentLocation).Size() * MoveCost <= ActionPoints;
    // return (newPos - CurrentLocation).Size() <= MoveRadius;
}

bool ATrooper::CheckAttackCorrectness(const FVector attackLocation,
                                      int abilityIndex) const {
    return GetAbility(abilityIndex) != nullptr && (
               attackLocation - CurrentLocation).Size() < GetActionRadius(
               abilityIndex);
    // return (attackLocation - CurrentLocation).Size() <= AttackRadius;
}
