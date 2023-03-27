// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Trooper.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

// DECLARE_DYNAMIC_DELEGATE_OneParam(FOnMyTurnChangedDelegate, bool, bMyTurn);

UCLASS()
class TURNBASEDTUTORIAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// FOnMyTurnChangedDelegate OnMyTurnChanged;

	virtual void SetupInputComponent() override;

	AMyPlayerController();

	UFUNCTION(Client, Reliable)
	void StartTurn();

	UFUNCTION(Client, Reliable)
	void EndTurn();

	UFUNCTION(Server, Reliable)
	void MoveTrooper(ATrooper* Trooper, FVector Location);

	UFUNCTION(Server, Reliable)
	void AttackTrooper(ATrooper* Attacker, ATrooper* Victim);

	UFUNCTION()
	void SetPlayerIndex(uint8 NewPlayerIndex);

private:
	bool bIsMyTurn;

	UPROPERTY(Replicated)
	uint8 PlayerIndex;

	ATrooper* SelectedTrooper;

	void OnLeftMouseClick();

	void SetMyTurn(bool bMyTurn);

	auto GetMyGameMode() const;
};
