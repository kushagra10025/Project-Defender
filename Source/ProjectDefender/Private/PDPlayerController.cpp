// (c) Kushagra, 2022


#include "PDPlayerController.h"

APDPlayerController::APDPlayerController()
{
	bShowMouseCursor = true;
}

void APDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CurrentMouseCursor = EMouseCursor::Crosshairs;
}
