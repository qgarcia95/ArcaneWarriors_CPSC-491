#include "Characters/ArcaneCharacter.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "GroomComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h" 
#include "HUD/ArcaneWarriorHUD.h"
#include "HUD/ArcaneWarriorOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

AArcaneCharacter::AArcaneCharacter()
	PrimaryActorTick.bCanEverTick = true;

	//Sets default yaw, pitch, roll values
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Used to set default character movement settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true); 

	// creating default CameraBoom component settings
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	// creating and attaching ViewCamera component and CameraBoom
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	// Creating hair groom component
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	// Creating eyebrows groom component
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void AArcaneCharacter::Tick(float DeltaTime)
{
	if (Attributes && ArcaneWarriorOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		ArcaneWarriorOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AArcaneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Binds axis mapping
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AArcaneCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AArcaneCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AArcaneCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AArcaneCharacter::LookUp);

	// bind action mapping
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AArcaneCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AArcaneCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AArcaneCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AArcaneCharacter::Dodge);
}

void AArcaneCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

float AArcaneCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AArcaneCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AArcaneCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AArcaneCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && ArcaneWarriorOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		ArcaneWarriorOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AArcaneCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && ArcaneWarriorOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		ArcaneWarriorOverlay->SetGold(Attributes->GetGold());
	}
}

void AArcaneCharacter::BeginPlay()
{
	Super::BeginPlay();	

	Tags.Add(FName("EngageableTarget"));
	InitializeArcaneWarriorOverlay();
}


void AArcaneCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// Find out which way is Forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AArcaneCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// Find out which way is Right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AArcaneCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AArcaneCharacter::LookUp(float Value)
{

	AddControllerPitchInput(Value);
}

void AArcaneCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void AArcaneCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking; 
	}
}

void AArcaneCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;

	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && ArcaneWarriorOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		ArcaneWarriorOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}


void AArcaneCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AArcaneCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AArcaneCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}

bool AArcaneCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AArcaneCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AArcaneCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AArcaneCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AArcaneCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AArcaneCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AArcaneCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AArcaneCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AArcaneCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool AArcaneCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool AArcaneCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void AArcaneCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AArcaneCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AArcaneCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AArcaneCharacter::InitializeArcaneWarriorOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AArcaneWarriorHUD* ArcaneWarriorHUD = Cast<AArcaneWarriorHUD>(PlayerController->GetHUD());
		if (ArcaneWarriorHUD)
		{
			ArcaneWarriorOverlay = ArcaneWarriorHUD->GetArcaneWarriorOverlay();
			if (ArcaneWarriorOverlay && Attributes)
			{
				ArcaneWarriorOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				ArcaneWarriorOverlay->SetStaminaBarPercent(1.f);
				ArcaneWarriorOverlay->SetGold(0);
				ArcaneWarriorOverlay->SetSouls(0);
			}
		}
	}
}

void AArcaneCharacter::SetHUDHealth()
{
	if (ArcaneWarriorOverlay && Attributes)
	{
		ArcaneWarriorOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
