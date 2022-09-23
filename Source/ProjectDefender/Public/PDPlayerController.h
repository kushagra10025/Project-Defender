// (c) Kushagra, 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDEFENDER_API APDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APDPlayerController();

	virtual void BeginPlay() override;
	
};
