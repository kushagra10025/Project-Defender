// (c) Kushagra, 2022


#include "PDPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	// FireModes = static_cast<uint8>(EFireMode::FM_Single) | static_cast<uint8>(EFireMode::FM_Burst) | static_cast<uint8>(EFireMode::FM_Auto);
	FireType = EFireType::Projectile;
	CurrentFireMode = EFireMode::Single;
	BurstFireCount = 3;
	TempBurstFireCount = BurstFireCount;
	BurstFireDuration = 0.15f;
	AutoFireDuration = 0.1f;

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
	CursorWorldDirection = 1000.0f * CursorWorldDirection;
	const FVector LookAtTargetLocation = CursorWorldPosition + CursorWorldDirection;
	if(bCanTurnRight && IsWorldDirectionAvailable)
	{
		const FVector InverseTransformDirection {GetActorTransform().InverseTransformVectorNoScale((LookAtTargetLocation-EndMesh->GetComponentLocation()))};
		float YawMakeRotFromX =  FRotationMatrix::MakeFromX(InverseTransformDirection).Rotator().Yaw;
		if(bClampTurnRotation)
		{
			YawMakeRotFromX = FMath::ClampAngle(YawMakeRotFromX, TurnRotationLimits.X, TurnRotationLimits.Y);
		}
		const FRotator Result {FRotator(0.0f, YawMakeRotFromX, 0.0f)};
		MidMesh->SetRelativeRotation(Result);
	}
}

void APDPlayerPawn::LookUp_Implementation(float Value)
{
	FVector CursorWorldPosition;
	FVector CursorWorldDirection;
	const bool IsWorldDirectionAvailable = UGameplayStatics::GetPlayerController(this, 0)->DeprojectMousePositionToWorld(CursorWorldPosition, CursorWorldDirection);
	CursorWorldDirection = 1000.0f * CursorWorldDirection;
	const FVector LookAtTargetLocation = CursorWorldPosition + CursorWorldDirection;
	if(bCanLookUp && IsWorldDirectionAvailable)
	{
		const FVector InverseTransformDirection {GetActorTransform().InverseTransformVectorNoScale((LookAtTargetLocation-EndMesh->GetComponentLocation()))};
		float PitchMakeRotFromX = FRotationMatrix::MakeFromX(InverseTransformDirection).Rotator().Pitch;
		if(bClampLookRotation)
		{
			PitchMakeRotFromX = FMath::ClampAngle(PitchMakeRotFromX, LookRotationLimits.X, LookRotationLimits.Y);
		}
		const FRotator Result {FRotator(PitchMakeRotFromX, 0.0f, 0.0f)};
		EndMesh->SetRelativeRotation(Result);
	}
}

void APDPlayerPawn::PrimaryAttack_Released_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_FireMode_Auto); // Clear and Invalidate Timer
}

void APDPlayerPawn::PrimaryAttack_Pressed_Implementation()
{
	switch(CurrentFireMode)
	{
	case EFireMode::None:
		checkNoEntry();
		break;
	case EFireMode::Single:
		FireWeapon();
		break;
	case EFireMode::Burst:
		GetWorldTimerManager().SetTimer(TimerHandle_FireMode_Burst, this, &APDPlayerPawn::FireMode_Burst, BurstFireDuration, true, 0.0f);
		break;
	case EFireMode::Auto:
		GetWorldTimerManager().SetTimer(TimerHandle_FireMode_Auto, this, &APDPlayerPawn::FireMode_Auto, AutoFireDuration, true, 0.0f);
		break;
	default:
		checkNoEntry();
	}
}

void APDPlayerPawn::FireWeapon()
{
	switch (FireType)
	{
	case EFireType::None:
		checkNoEntry();
		break;
	case EFireType::Projectile:
		ProjectileFire();
		break;
	case EFireType::Raycast:
		HitscanFire();
		break;
	default:
		checkNoEntry();
	}
}

void APDPlayerPawn::ProjectileFire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Projectile Fire"));
}

void APDPlayerPawn::HitscanFire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hitscan Fire"));
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

FRotator APDPlayerPawn::GetCameraBoomYawRotation(float RotationSpeed) const
{
	const FRotator CameraBoomRotation {CameraBoom->GetComponentRotation()};
	const FRotator Result {FRotator(CameraBoomRotation.Pitch, CameraBoomRotation.Yaw + RotationSpeed, CameraBoomRotation.Roll)};
	return Result;
}

void APDPlayerPawn::FireMode_Auto()
{
	// UE_LOG(LogTemp, Warning, TEXT("Auto Fire"));
	FireWeapon();
}

void APDPlayerPawn::FireMode_Burst()
{
	// UE_LOG(LogTemp, Warning, TEXT("Burst Fire"));
	if(!(TempBurstFireCount > 0))
	{
		TempBurstFireCount = BurstFireCount;
		GetWorldTimerManager().ClearTimer((TimerHandle_FireMode_Burst));
	}
	FireWeapon();
	TempBurstFireCount = FMath::Max(TempBurstFireCount - 1, 0);
}

void APDPlayerPawn::ChangeFireMode()
{
	switch(CurrentFireMode)
	{
	case EFireMode::None:
		checkNoEntry();
		break;
	case EFireMode::Single:
		CurrentFireMode = EFireMode::Burst;
		break;
	case EFireMode::Burst:
		CurrentFireMode = EFireMode::Auto;
		break;
	case EFireMode::Auto:
		CurrentFireMode = EFireMode::Single;
		break;
	default:
		checkNoEntry();
	}
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

	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Pressed, this, &APDPlayerPawn::PrimaryAttack_Pressed);
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Released, this, &APDPlayerPawn::PrimaryAttack_Released);

	PlayerInputComponent->BindAction("FireMode", EInputEvent::IE_Pressed, this, &APDPlayerPawn::ChangeFireMode);
}

