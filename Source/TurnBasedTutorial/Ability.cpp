// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability.h"

// Sets default values for this component's properties
UAbility::UAbility() {
    PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UAbility::BeginPlay() {
    Super::BeginPlay();

}

// void UAbility::TickComponent(float DeltaTime,
//                              ELevelTick TickType,
//                              FActorComponentTickFunction *ThisTickFunction) {
//     Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// }
