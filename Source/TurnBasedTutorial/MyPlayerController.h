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

	AMyPlayerController();

	UFUNCTION(Client, Reliable)
	void StartTurn();

	UFUNCTION(Client, Reliable)
	void EndTurn();

	UFUNCTION(Server, Reliable)
	void MoveHero();

	virtual void SetupInputComponent() override;

	void SetTrooperIsMoving(bool isMoving);

private:
	bool bIsMyTurn;

	bool bIsThereTrooperMoving = false;

	ATrooper* SelectedTrooper;

	void OnLeftMouseClick();

	void SetMyTurn(bool bMyTurn);

	auto GetMyGameMode() const;
};
