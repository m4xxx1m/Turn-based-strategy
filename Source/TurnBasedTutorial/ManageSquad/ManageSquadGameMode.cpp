#include "ManageSquadGameMode.h"

#include "ManageSquadGameState.h"
#include "ManageSquadPawn.h"
#include "ManageSquadPlayerController.h"

AManageSquadGameMode::AManageSquadGameMode() {
    PlayerControllerClass = AManageSquadPlayerController::StaticClass();
    DefaultPawnClass = AManageSquadPawn::StaticClass();
    GameStateClass = AManageSquadGameState::StaticClass();
}
