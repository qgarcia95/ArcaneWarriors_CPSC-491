#include "Characters/ArcaneAnimInstance.h"
#include "Characters/ArcaneCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"

void UArcaneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ArcaneCharacter = Cast<AArcaneCharacter>(TryGetPawnOwner());
	if (ArcaneCharacter)
	{
		ArcaneCharacterMovement = ArcaneCharacter->GetCharacterMovement();
	}
}

void UArcaneAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime); 

	if (ArcaneCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ArcaneCharacterMovement->Velocity);
		IsFalling = ArcaneCharacterMovement->IsFalling();
		CharacterState = ArcaneCharacter->GetCharacterState();
		ActionState = ArcaneCharacter->GetActionState();
		DeathPose = ArcaneCharacter->GetDeathPose();
	}
}
