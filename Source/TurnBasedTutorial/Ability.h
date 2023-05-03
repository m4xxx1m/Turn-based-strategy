// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDTUTORIAL_API UAbility : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UAbility();

    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    float ActionCost = 100.0f;

    UPROPERTY(EditAnywhere)
    float Damage = 50.0f;

    UPROPERTY(EditAnywhere)
    float ActionRadius = 1000.0f;

    UPROPERTY(EditAnywhere)
    float SplashRadius = 100.0f;

    UPROPERTY(EditAnywhere)
    float LinearWidth = 50.0f;
    
    // Called every frame
    // virtual void TickComponent(float DeltaTime,
    //                            ELevelTick TickType,
    //                            FActorComponentTickFunction *
    //                            ThisTickFunction) override;
};
