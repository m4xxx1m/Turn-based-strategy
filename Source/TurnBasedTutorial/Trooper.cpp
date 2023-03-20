#include "Trooper.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"

// Sets default values
ATrooper::ATrooper()
{
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

void ATrooper::Tick(float const DeltaTime)
{
	if (bIsMoving)
	{
		FVector vector = (MoveToVector - Position);
		vector.Normalize();
		vector *= (Speed * DeltaTime);
		if (vector.Size() >= (MoveToVector - Position).Size())
		{
			Position = MoveToVector;
			bIsMoving = false;
			dynamic_cast<AMyPlayerController*>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0)
			)->SetTrooperIsMoving(false);
		}
		else
		{
			Position += vector;
		}
		SetActorLocation(Position);
	}
}

void ATrooper::MoveTrooper(FVector const NewPos)
{
	MoveToVector = NewPos;
	bIsMoving = true;
}

uint8 ATrooper::NumberOfTroopersForId = 0;

void ATrooper::InitNumberOfTroopersForId()
{
	NumberOfTroopersForId = 0;
}

FVector ATrooper::GetPosition() const
{
	return Position;
}

bool ATrooper::IsOnPlayersSide() const
{
	return bOnPlayersSide;
}

uint8 ATrooper::GetId() const
{
	return Id;
}

void ATrooper::InitTrooper(FVector const NewPosition, bool const bIsOnPlayersSide)
{
	Position = NewPosition;
	bOnPlayersSide = bIsOnPlayersSide;
}
