#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArcaneWarriorHUD.generated.h"

class UArcaneWarriorOverlay;

UCLASS()
class ARCANEWARRIORS_API AArcaneWarriorHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arcane Warrior")
	TSubclassOf<UArcaneWarriorOverlay> ArcaneWarriorOverlayClass;

	UPROPERTY()
	UArcaneWarriorOverlay* ArcaneWarriorOverlay;
public:
	FORCEINLINE UArcaneWarriorOverlay* GetArcaneWarriorOverlay() const { return ArcaneWarriorOverlay; }
};
