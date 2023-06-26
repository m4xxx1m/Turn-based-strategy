// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ManageSquadTrooper.generated.h"

UENUM()
enum ETrooperType {
    TROOPER_SAMPLE,
    TROOPER_IN_SQUAD
};

UCLASS()
class TURNBASEDTUTORIAL_API AManageSquadTrooper : public ACharacter {
    GENERATED_BODY()

    AManageSquadTrooper();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    int Index = -1;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<ETrooperType> Type;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void ChangeSkeletalMesh(const AManageSquadTrooper *OtherTrooper) const;

    UFUNCTION()
    ETrooperType GetType() const;

    UFUNCTION()
    int GetIndex() const;

    UFUNCTION()
    void Initialize(int TrooperIndex);
};
