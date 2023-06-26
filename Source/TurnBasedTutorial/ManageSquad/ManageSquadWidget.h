// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManageSquadWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UManageSquadWidget : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *BackButton;
    
    UFUNCTION()
    void OnBackButtonClicked();
};
