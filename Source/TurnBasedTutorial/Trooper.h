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

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float const DeltaTime) override;

    // void SetStaticMesh() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent *MyStaticMesh;

    const TCHAR *MeshPath = nullptr;

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
