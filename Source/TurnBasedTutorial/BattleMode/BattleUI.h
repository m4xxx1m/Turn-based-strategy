// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UBattleUI : public UUserWidget {
    GENERATED_BODY()
    
public:
    virtual void NativeConstruct() override;
    
    UFUNCTION(Client, Reliable)
    void SetWhoseTurnText(bool IsThisPlayerTurn);
    
    UFUNCTION(Client, Reliable)
    void SetWidgetText(const FString &Text);
    
protected:
    int ActionType = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *EndTurnButton;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *ButtonAction_0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *ButtonAction_1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *ButtonAction_2;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock *InformationText;

    UFUNCTION()
    void OnEndTurnClicked();
    
    UFUNCTION()
    void OnActionSwitched_0();
    
    UFUNCTION()
    void OnActionSwitched_1();
    
    UFUNCTION()
    void OnActionSwitched_2();

    UFUNCTION()
    void OnActionSwitched() const;

};
