#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "GameFramework/Character.h"
#include "Trooper.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API ATrooper : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATrooper();

    void Initialize(uint8 const NewPlayerIndex,
                    FVector const SpawnLocation,
                    uint8 const NewId);

    UFUNCTION()
    uint8 GetPlayerIndex() const;

    UFUNCTION()
    void MoveTrooper(FVector const NewPos);

    UFUNCTION()
    uint8 GetId() const;

    UFUNCTION()
    bool CheckMoveCorrectness(const FVector newPos) const;

    UFUNCTION()
    bool CheckAttackCorrectness(const FVector attackLocation, int abilityIndex) const;

    UFUNCTION()
    FVector GetLocation() const;

    UFUNCTION(BlueprintCallable)
    int GetAnimationValue();
    
    UFUNCTION()
    void Attack(int AbilityIndex, FVector ToLocation);
    
    UFUNCTION()
    float GetActionRadius(int action) const;

    UFUNCTION()
    float GetHitPoints() const;

    UFUNCTION()
    float GetMaxHitPoints() const;

    UFUNCTION()
    void SetSelection(bool Selected, uint8 ActionType) const;

    UFUNCTION()
    void UpdateSelectionRadius(uint8 ActionType) const;

    UFUNCTION(Client, Reliable)
    void HighlightAsEnemy(int8 Index) const;
    
    UFUNCTION()
    void ResetActionPoints();

    UFUNCTION()
    UAbility *GetAbility(int AbilityIndex) const;

    UFUNCTION()
    bool TakeDamage(float Damage);

protected:
    constexpr static float PIXELS_IN_RADIUS = 50;
    
    UPROPERTY(EditAnywhere)
    UMaterialInterface *GreenMaterial = nullptr;

    UPROPERTY(EditAnywhere)
    UMaterialInterface *RedMaterial = nullptr;
    
    UPROPERTY(EditAnywhere)
    UAbility *AttackAbility;

    UPROPERTY(EditAnywhere)
    UAbility *SpecialAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AMyProjectile> AttackProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AMyProjectile> SpecialProjectileClass;

    UFUNCTION()
    TSubclassOf<AMyProjectile> GetProjectileClass(uint8 AbilityIndex) const;

    UFUNCTION(Server, Reliable)
    void FireProjectile();

    UPROPERTY(Replicated)
    uint8 CurrentAbilityIndex = -1;

    UPROPERTY(Replicated)
    FVector CurrentAbilityDestination = {};
    
    UPROPERTY(EditAnywhere)
    float Speed = 300.0f;

    UPROPERTY(EditAnywhere)
    float StartHitPoints = 100.0f;

    UPROPERTY(EditAnywhere)
    float MoveCost = 0.0667f;

    UPROPERTY(EditAnywhere)
    float StartActionPoints = 100.0f;
    
    UPROPERTY(Replicated)
    float HitPoints;

    UPROPERTY(Replicated)
    float ActionPoints;
    
    UPROPERTY(Replicated)
    bool bIsAttacking = false;

    UPROPERTY(Replicated)
    float AttackPlayedTime;

    const float AttackDuration = 1.16667f;

    const float FireAfterTime = 0.6f;

    UPROPERTY(Replicated)
    bool bIsWaitingForFire = false;

    UPROPERTY(Replicated)
    bool bIsTakingDamage = false;
    
    UPROPERTY(Replicated)
    float TakingDamagePlayedTime;

    const float TakingDamageDuration = 1.46667f;
    
    UPROPERTY(Replicated)
    bool bIsDead = false;
    
    virtual void BeginPlay() override;

    virtual void Tick(float const DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Replicated)
    class UWidgetComponent *HealthWidgetComponent;

    // void SetStaticMesh() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent *SelectionStaticMesh;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // USkeletalMeshComponent *MySkeletalMesh;

    // const TCHAR *MeshPath = nullptr;

    // UFUNCTION()
    // void OnRepNotify_PlayerIndex() const;
    
    UPROPERTY(Replicated/*Using = OnRepNotify_PlayerIndex*/)
    int8 PlayerIndex = -1;
    
    UPROPERTY(Replicated)
    uint8 Id;
    
    UPROPERTY(Replicated)
    FVector CurrentLocation;

    UPROPERTY(Replicated)
    FVector TargetLocation;

    UPROPERTY(Replicated)
    bool bIsMoving = false;
};

// UCLASS()
// class ATrooperWizard : public ATrooper {
//     GENERATED_BODY()
//
// public:
//     ATrooperWizard();
//     
// };
//
// UCLASS()
// class ATrooperSkeletonMelee : public ATrooper {
//     GENERATED_BODY()
//
// public:
//     ATrooperSkeletonMelee();
//     
// };
