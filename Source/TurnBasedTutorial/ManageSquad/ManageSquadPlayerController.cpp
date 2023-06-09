// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadPlayerController.h"

#include "ManageSquadGameState.h"
#include "ManageSquadWidget.h"

AManageSquadPlayerController::AManageSquadPlayerController() {
    SetShowMouseCursor(true);
}

void AManageSquadPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("MyAction", IE_Pressed, this,
                               &AManageSquadPlayerController::OnLeftMouseClick);
}

void AManageSquadPlayerController::BeginPlay() {
    Super::BeginPlay();
    const TSoftClassPtr<UUserWidget> WidgetClass = TSoftClassPtr<
        UUserWidget>(FSoftObjectPath(
        "WidgetBlueprint'/Game/ManageSquadMenu/BP_ManageSquadWidget.BP_ManageSquadWidget_C'"
    ));
    UUserWidget *CreatedWidget = CreateWidget<UUserWidget>(
        GetWorld(), WidgetClass.LoadSynchronous());
    if (CreatedWidget) {
        CreatedWidget->AddToViewport();
    }
}

void AManageSquadPlayerController::OnLeftMouseClick() {
    UE_LOG(LogTemp, Warning, TEXT("Mouse clicked"));
    FHitResult HitResult;
    bool const IsHitResult = GetHitResultUnderCursorForObjects(
        TArray<TEnumAsByte<EObjectTypeQuery>>{ObjectTypeQuery1}, false,
        HitResult);
    if (!IsHitResult)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Got hit result"));
    // auto const NewlySelectedLocation = HitResult.Location;
    AManageSquadTrooper *NewlySelectedTrooper = Cast
        <AManageSquadTrooper>(
            HitResult.GetActor());

    if (NewlySelectedTrooper == nullptr || !NewlySelectedTrooper->
        IsValidLowLevel()) {
        // we selected something that is not a trooper (or trooper in shitty state...)
        UE_LOG(LogTemp, Warning, TEXT("Not a trooper"));
        return;
    }
    // skip re-selection
    if (SelectedTrooper == NewlySelectedTrooper) {
        UE_LOG(LogTemp, Warning, TEXT("Skip reselection"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Trooper"));
    switch (NewlySelectedTrooper->GetType()) {
        case ETrooperType::TROOPER_SAMPLE:
            if (SelectedTrooper) {
                UE_LOG(LogTemp, Warning, TEXT("Trooper replacement"));
                SelectedTrooper->ChangeSkeletalMesh(NewlySelectedTrooper);
                Cast<AManageSquadGameState>(
                    GetWorld()->GetGameState())->ChangeSquad(
                    SelectedTrooper->GetIndex(),
                    NewlySelectedTrooper->GetIndex()
                );
            }
            break;
        case ETrooperType::TROOPER_IN_SQUAD:
            SelectedTrooper = NewlySelectedTrooper;
            UE_LOG(LogTemp, Warning, TEXT("Changed selection"));
            break;
        default:
            break;
    }
}
