#include "HUD/ArcaneWarriorHUD.h"
#include "HUD/ArcaneWarriorOverlay.h"

void AArcaneWarriorHUD::BeginPlay()
{
	Super::BeginPlay(); 

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && ArcaneWarriorOverlayClass)
		{
			ArcaneWarriorOverlay = CreateWidget<UArcaneWarriorOverlay>(Controller, ArcaneWarriorOverlayClass); 
			ArcaneWarriorOverlay->AddToViewport();
		}
	}
}
