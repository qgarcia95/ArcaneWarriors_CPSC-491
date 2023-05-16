#include "HUD/ArcaneWarriorOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UArcaneWarriorOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UArcaneWarriorOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UArcaneWarriorOverlay::SetGold(int32 Gold)
{ 
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String); 
		GoldText->SetText(Text);
	}
}

void UArcaneWarriorOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
