// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController() : Super(), IsMyTurn(false), SelectedTrooper(nullptr) {
	UE_LOG(LogTemp, Warning, TEXT("Player controller created"));
}

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction("MyAction", IE_Pressed, this,
		&AMyPlayerController::OnLeftMouseClick);
}

void AMyPlayerController::StartTurn() {
	IsMyTurn = true;
	UE_LOG(LogTemp, Warning, TEXT("Your turn"));
}

void AMyPlayerController::EndTurn() {
	IsMyTurn = false;
	UE_LOG(LogTemp, Warning, TEXT("Not your turn"));
}

void AMyPlayerController::SetTrooperIsMoving(bool isMoving) {
	IsThereTrooperMoving = isMoving;
}

void AMyPlayerController::OnLeftMouseClick() {
	if (IsThereTrooperMoving) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Mouse clicked"));
	FHitResult HitResult;
	bool IsHitResult = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, HitResult);
	if (IsHitResult) {
		AActor *actor = HitResult.Actor.Get();
		if (actor->ActorHasTag(FName("Trooper"))) {
			ATrooper* trooper = dynamic_cast<ATrooper*>(actor);
			if (trooper != nullptr && trooper != SelectedTrooper) {
				if (trooper->IsOnPlayersSide()) {
					UE_LOG(LogTemp, Warning, TEXT("Hitted trooper id: %d, on our side"),
						trooper->GetId());
					SelectedTrooper = trooper;
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("Hitted trooper id: %d, enemy"),
						trooper->GetId());
					UE_LOG(LogTemp, Warning, TEXT("Trooper #%d hit enemy trooper #%d"),
						SelectedTrooper->GetId(), trooper->GetId());
				}
			}
		}
		else if (actor->ActorHasTag(FName("Floor"))) {
			UE_LOG(LogTemp, Warning, TEXT("Hitted floor: %f, %f, %f"), HitResult.Location.X, 
				HitResult.Location.Y, HitResult.Location.Z);
			if (SelectedTrooper != nullptr) {
				SelectedTrooper->MoveTrooper(HitResult.Location);
				IsThereTrooperMoving = true;
			}
		}
	}
}
