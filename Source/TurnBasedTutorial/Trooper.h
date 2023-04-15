#pragma once

#include "CoreMinimal.h"
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
    bool CheckAttackCorrectness(const FVector attackLocation) const;

    UFUNCTION()
    FVector GetLocation() const;

    UFUNCTION(BlueprintCallable)
    float GetAnimationValue();

    UFUNCTION()
    void Attack();
    
protected:
    const float MoveRadius = 1500.f;

    const float AttackRadius = 1000.f;

    UPROPERTY(Replicated)
    bool bIsAttacking = false;

    UPROPERTY(Replicated)
    float AttackPlayedTime;
    
    const float AttackDuration = 1.16667f;

    virtual void BeginPlay() override;

    virtual void Tick(float const DeltaTime) override;

    // void SetStaticMesh() const;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    // UStaticMeshComponent *MyStaticMesh;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // USkeletalMeshComponent *MySkeletalMesh;

    // const TCHAR *MeshPath = nullptr;

    UPROPERTY(Replicated)
    uint8 PlayerIndex;

    UPROPERTY(Replicated)
    uint8 Id;

    UPROPERTY()
    float Speed = 300.0f;

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
