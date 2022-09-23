// (c) Kushagra, 2022


#include "PDPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APDPlayerPawn::APDPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CollisionComponent);

	MidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MidMesh"));
	MidMesh->SetupAttachment(BaseMesh);

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndMesh"));
	EndMesh->SetupAttachment(MidMesh, FName("AttachGunPoint"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CollisionComponent);

	LookCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LookCamera"));
	LookCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
}

void APDPlayerPawn::TurnRight_Implementation(float Value)
{
}

void APDPlayerPawn::LookUp_Implementation(float Value)
{
}

// Called when the game starts or when spawned
void APDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APDPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("TurnRight", this, &APDPlayerPawn::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APDPlayerPawn::LookUp);

}

