#include "Trooper.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"

// Sets default values
ATrooper::ATrooper() {
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Trooper"));
	Id = NumberOfTroopersForId++;
	Position.Set(0, 0, 0);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT(
		"StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"
	));
	if (MeshToUse.Object)
	{
		Mesh->SetStaticMesh(MeshToUse.Object);
	}
}

// Called when the game starts or when spawned
void ATrooper::BeginPlay()
{
	Super::BeginPlay();
}

void ATrooper::Tick(float deltaTime) {
	if (IsMoving) {
		FVector vector = (MoveToVector - Position);
		vector.Normalize();
		vector *= (Speed * deltaTime);
		if (vector.Size() >= (MoveToVector - Position).Size()) {
			Position = MoveToVector;
			IsMoving = false;
			dynamic_cast<AMyPlayerController*>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0)
				)->SetTrooperIsMoving(false);
		}
		else {
			Position += vector;
		}
		SetActorLocation(Position);
	}
}

void ATrooper::MoveTrooper(FVector newPos) {
	MoveToVector = newPos;
	IsMoving = true;
}

int ATrooper::NumberOfTroopersForId = 0;

void ATrooper::InitNumberOfTroopersForId() {
	NumberOfTroopersForId = 0;
}

FVector ATrooper::GetPosition() {
	return Position;
}

bool ATrooper::IsOnPlayersSide() {
	return OnPlayersSide;
}

int ATrooper::GetId() {
	return Id;
}

void ATrooper::InitTrooper(FVector position, bool onPlayersSide) {
	Position = position;
	OnPlayersSide = onPlayersSide;
}
