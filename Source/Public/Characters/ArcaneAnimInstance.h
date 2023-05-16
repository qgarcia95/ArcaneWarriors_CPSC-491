#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "ArcaneAnimInstance.generated.h"

UCLASS()
class ARCANEWARRIORS_API UArcaneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly);
	class AArcaneCharacter* ArcaneCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	class UCharacterMovementComponent* ArcaneCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose; 	
};
