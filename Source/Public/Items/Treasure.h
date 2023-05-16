#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

UCLASS()
class ARCANEWARRIORS_API ATreasure : public AItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
