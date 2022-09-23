// (c) Kushagra, 2022


#include "PDPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 10.0f;

	LookCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LookCamera"));
	LookCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bEnableCameraEdgeRotation = true;
	ScreenHospotZone = 0.02;

	bCanTurnRight = true;
	bClampTurnRotation = false;
	TurnRotationLimits = FVector2D(-20.0f, 20.0f);

	bCanLookUp = true;
	bClampLookRotation = false;
	LookRotationLimits = FVector2D(-20.0f, 20.0f);

}

void APDPlayerPawn::TurnRight_Implementation(float Value)
{
	if(bEnableCameraEdgeRotation)
	{
		CameraEdgeRotation();
	}

	FVector CursorWorldPosition;
	FVector CursorWorldDirection;
	const bool IsWorldDirectionAvailable = UGameplayStatics::GetPlayerController(this, 0)->DeprojectMousePositionToWorld(CursorWorldPosition, CursorWorldDirection);
	if(bCanTurnRight && IsWorldDirectionAvailable)
	{
		const FRotator MidMeshRot {MidMesh->GetComponentRotation()};
		float YawMakeRotFromX =  FRotationMatrix::MakeFromX(CursorWorldDirection).Rotator().Yaw;
		if(bClampTurnRotation)
		{
			YawMakeRotFromX = FMath::ClampAngle(YawMakeRotFromX, TurnRotationLimits.X, TurnRotationLimits.Y);
		}
		const FRotator Result {FRotator(MidMeshRot.Pitch, YawMakeRotFromX, MidMeshRot.Roll)};
		MidMesh->SetWorldRotation(Result);
	}
}

void APDPlayerPawn::LookUp_Implementation(float Value)
{
	FVector CursorWorldPosition;
	FVector CursorWorldDirection;
	const bool IsWorldDirectionAvailable = UGameplayStatics::GetPlayerController(this, 0)->DeprojectMousePositionToWorld(CursorWorldPosition, CursorWorldDirection);
	if(bCanLookUp && IsWorldDirectionAvailable)
	{
		const FRotator EndMeshRot {EndMesh->GetComponentRotation()};
		float PitchMakeRotFromX = FRotationMatrix::MakeFromX(CursorWorldDirection).Rotator().Pitch;
		if(bClampLookRotation)
		{
			PitchMakeRotFromX = FMath::ClampAngle(PitchMakeRotFromX, LookRotationLimits.X, LookRotationLimits.Y);
		}
		const FRotator Result {FRotator(PitchMakeRotFromX, EndMeshRot.Yaw, EndMeshRot.Roll)};
		EndMesh->SetWorldRotation(Result);
	}
}

void APDPlayerPawn::CameraEdgeRotation()
{
	FVector2D MousePosition;
	FVector2D ViewportSize;

	if(!UGameplayStatics::GetPlayerController(this,0)->GetMousePosition(MousePosition.X, MousePosition.Y))
		return;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	const float MouseToScreenPercent = (MousePosition.X / ViewportSize.X);
	if(MouseToScreenPercent > (1-ScreenHospotZone))
	{
		CameraBoom->SetWorldRotation(GetCameraBoomYawRotation(5.0f));
	}else if(MouseToScreenPercent < ScreenHospotZone)
	{
		CameraBoom->SetWorldRotation(GetCameraBoomYawRotation(-5.0f));
	}else
	{
		// In Middle Region
	}
}

const FRotator APDPlayerPawn::GetCameraBoomYawRotation(float RotationSpeed)
{
	const FRotator CameraBoomRotation {CameraBoom->GetComponentRotation()};
	const FRotator Result {FRotator(CameraBoomRotation.Pitch, CameraBoomRotation.Yaw + RotationSpeed, CameraBoomRotation.Roll)};
	return Result;
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

