// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trooper.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TURNBASEDTUTORIAL_API UHealthBar : public UUserWidget {
    GENERATED_BODY()
public:
    void SetOwnerTrooper(ATrooper *Trooper);
    
protected:
    TWeakObjectPtr<ATrooper> OwnerTrooper;
    
    UPROPERTY(meta=(BindWidget))
    class UProgressBar *HealthBar;

    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
};
