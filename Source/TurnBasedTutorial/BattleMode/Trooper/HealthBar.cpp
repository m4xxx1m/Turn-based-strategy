// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include "Components/ProgressBar.h"

void UHealthBar::SetOwnerTrooper(ATrooper *Trooper) {
    OwnerTrooper = Trooper;
}

void UHealthBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!OwnerTrooper.IsValid())
        return;

    HealthBar->SetPercent(
        OwnerTrooper->GetHitPoints() / OwnerTrooper->GetMaxHitPoints());
}
