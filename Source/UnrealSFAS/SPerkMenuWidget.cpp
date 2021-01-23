// Fill out your copyright notice in the Description page of Project Settings.


#include "SPerkMenuWidget.h"
#include "MenuHUD.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <sstream>
#include <fstream>
#include "Widgets/Input/SEditableTextBox.h"



void SPerkMenuWidget::Construct(const FArguments& InArgs)
{
	/*Sets the focus for the menu to be true*/
	bCanSupportFocus = true;
	/*Assigns the owningHUD with the MenuHUD*/
	OwningHUD = InArgs._OwningHUD;
	/*Gets The current game instance*/
	OwningHUD->SetInstance();
	/*This will set up the text and font*/
	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.f;

	/*The Title Text setup*/
	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.f;

	/*The Title Text setup*/
	FSlateFontInfo TextStyle = ButtonTextStyle;
	TextStyle.Size = 40.f;

	ChildSlot
		[
			/* Gold Text */
			SNew(SOverlay) + SOverlay::Slot()
			[
				SNew(STextBlock).Font(TitleTextStyle).Text(this, &SPerkMenuWidget::GetGoldText).Justification(ETextJustify::Right)
			]
	+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
		[

			//Title Text
			SNew(SVerticalBox) + SVerticalBox::Slot()
			[
				SNew(STextBlock).Font(TitleTextStyle).Text(TitleText).Justification(ETextJustify::Center)
			]
			// Health Upgrade Button
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(SButton).OnClicked(this, &SPerkMenuWidget::UpgradeDamage)
					[

						SNew(STextBlock).Font(ButtonTextStyle).Text(this, &SPerkMenuWidget::GetSlot1Text).Justification(ETextJustify::Center)

					]
				]
			// Ammo Upgrade Button
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(SButton).OnClicked(this, &SPerkMenuWidget::UpgradeAmmo)
					[

						SNew(STextBlock).Font(ButtonTextStyle).Text(this, &SPerkMenuWidget::GetSlot2Text).Justification(ETextJustify::Center)

					]
				]
			// Speed Upgrade Button
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(SButton).OnClicked(this, &SPerkMenuWidget::UpgradeSpeed)
					[

						SNew(STextBlock).Font(ButtonTextStyle).Text(this, &SPerkMenuWidget::GetSlot3Text).Justification(ETextJustify::Center)

					]
				]
			// Return to menu Button
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
				SNew(SButton).OnClicked(this, &SPerkMenuWidget::ReturnToMenuClicked)
					[

					SNew(STextBlock).Font(ButtonTextStyle).Text(QuitText).Justification(ETextJustify::Center)

					]
				]
			/*Notification Title Text*/
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(STextBlock).Font(TitleTextStyle).Text(ConstNotificationText).Justification(ETextJustify::Center)
				]
			/*Notification Text*/
			+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(SButton)
					[
						SNew(STextBlock).Font(TextStyle).Text(this, &SPerkMenuWidget::GetCurrentNotificationText).Justification(ETextJustify::Center)
					]
				]

		]
		];

}
// Gets the slot 1 text
FText SPerkMenuWidget::GetSlot1Text() const
{
	/*This function will get what the current health counter is and display the text depending on that*/
	FString TempString;
	/*Depending on the counter, display the appropite message*/
	if (OwningHUD->GInstance->DamageUpgradeCounter == 0)
	{
		TempString = UpgradeDamageText + UpgradeTier1;
	}
	else if (OwningHUD->GInstance->DamageUpgradeCounter == 1)
	{
		TempString = UpgradeDamageText + UpgradeTier2;
	}
	else if (OwningHUD->GInstance->DamageUpgradeCounter == 2)
	{
		TempString = UpgradeDamageText + UpgradeTier3;
	}
	else
	{
		TempString = UpgradeDamageText + UpgradeMAX;
	}
	/**************************************************************/
	FText ReturnText = FText::FromString(TempString); // Assign the return text and return for it to be displayed
	return ReturnText;
}

/*Function is the same as the above but assigns different text, see GetSlot1Text*/
FText SPerkMenuWidget::GetSlot2Text() const
{
	FString TempString;
	if (OwningHUD->GInstance->AmmoUpgradeCounter == 0)
	{
		TempString = UpgradeAmmoText + UpgradeTier1;
	}
	else if (OwningHUD->GInstance->AmmoUpgradeCounter == 1)
	{
		TempString = UpgradeAmmoText + UpgradeTier2;
	}
	else if (OwningHUD->GInstance->AmmoUpgradeCounter == 2)
	{
		TempString = UpgradeAmmoText + UpgradeTier3;
	}
	else
	{
		TempString = UpgradeAmmoText + UpgradeMAX;
	}
	FText ReturnText = FText::FromString(TempString);
	return ReturnText;
}
/*Function is the same as the above but assigns different text, see GetSlot1Text*/
FText SPerkMenuWidget::GetSlot3Text() const
{
	FString TempString;
	if (OwningHUD->GInstance->SpeedUpgradeCounter == 0)
	{
		TempString = UpgradeSpeedText + UpgradeTier1;
	}
	else if (OwningHUD->GInstance->SpeedUpgradeCounter == 1)
	{
		TempString = UpgradeSpeedText + UpgradeTier2;
	}
	else if (OwningHUD->GInstance->SpeedUpgradeCounter == 2)
	{
		TempString = UpgradeSpeedText + UpgradeTier3;
	}
	else
	{
		TempString = UpgradeSpeedText + UpgradeMAX;
	}
	FText ReturnText = FText::FromString(TempString);
	return ReturnText;
}

/*This function will get the CurrentNoificationText*/
FText SPerkMenuWidget::GetCurrentNotificationText() const
{
	/*Depending on the Notification Index display the assigned Notification*/
	for (int i = 0; i < 3; i++)
	{
		if (OwningHUD->NotificationIndex == i)
		{
			FString sTemp = NotificationArray[i];
			FText fTemp = FText::FromString(sTemp);
			return fTemp;
		}
	}

	//if (OwningHUD->NotificationIndex == 1)
	//{
	//	FString sTemp = NotificationOne;
	//	FText fTemp = FText::FromString(sTemp);
	//	return fTemp;
	//}
	//else if (OwningHUD->NotificationIndex == 2)
	//{
	//	FString sTemp = NotificationTwo;
	//	FText fTemp = FText::FromString(sTemp);
	//	return fTemp;
	//}
	return FText();
}

/*Gets the current Gold text*/
FText SPerkMenuWidget::GetGoldText() const
{
	FText ReturnText = FText::FromString("GOLD: " + FString::FromInt(OwningHUD->GetPlayerGold()));
	return ReturnText;
}

/*This function will upgrade the damage if it is able too*/
FReply SPerkMenuWidget::UpgradeDamage() const
{
	/*If the counter is greater then 2 then we are at max and can't upgrade*/
	if (OwningHUD->GInstance->DamageUpgradeCounter <= 2)
	{
		int Price = 0;
		/*Gets the current Gold*/
		int CurrentGold = OwningHUD->GetPlayerGold();
		/*Depending on the current counter, set that to be the price*/
		if (OwningHUD->GInstance->DamageUpgradeCounter == 0) { Price = OwningHUD->Tier1Price; }
		else if (OwningHUD->GInstance->DamageUpgradeCounter == 1) { Price = OwningHUD->Tier2Price; }
		else if (OwningHUD->GInstance->DamageUpgradeCounter == 2) { Price = OwningHUD->Tier3Price; }
		/*************************************************************/
		/*If the player has enough gold then they can buy the perk*/
		if (CurrentGold >= Price)
		{
			/*This will set the notification, Add the upgrade, remove the gold, set the new gold and upgrade the counter*/
			OwningHUD->NotificationIndex = 1;
			OwningHUD->AddUpgrade(0);
			int NewGoldAmount = CurrentGold -= Price;
			OwningHUD->SetGold(NewGoldAmount);
			OwningHUD->GInstance->DamageUpgradeCounter++;
			/****************************************************************************/
		}
		else
		{
			/*If the player doesn't have enough gold then display notification 2*/
			OwningHUD->NotificationIndex = 2;
		}
	}
	return FReply::Handled();
}

/*This function will upgrade the Ammo if it is able too*/
/*This function is same as Upgrade Damage but is different because it upgrades the Ammo, See UpgradeDamage for notes*/
FReply SPerkMenuWidget::UpgradeAmmo() const
{
	if (OwningHUD->GInstance->AmmoUpgradeCounter <= 2)
	{
		int Price = 0;
		int CurrentGold = OwningHUD->GetPlayerGold();
		if (OwningHUD->GInstance->AmmoUpgradeCounter == 0) { Price = OwningHUD->Tier1Price; }
		else if (OwningHUD->GInstance->AmmoUpgradeCounter == 1) { Price = OwningHUD->Tier2Price; }
		else if (OwningHUD->GInstance->AmmoUpgradeCounter == 2) { Price = OwningHUD->Tier3Price; }
		if (CurrentGold >= Price)
		{
			OwningHUD->AddUpgrade(1);
			OwningHUD->NotificationIndex = 1;
			int NewGoldAmount = CurrentGold -= Price;
			OwningHUD->SetGold(NewGoldAmount);
			OwningHUD->GInstance->AmmoUpgradeCounter++;
		}
		else
		{
			OwningHUD->NotificationIndex = 2;
		}
	}
	return FReply::Handled();
}

/*This function will upgrade the Speed if it is able too*/
/*This function is same as Upgrade Damage but is different because it upgrades the Speed, See UpgradeDamage for notes*/
FReply SPerkMenuWidget::UpgradeSpeed() const
{
	if (OwningHUD->GInstance->SpeedUpgradeCounter <= 2)
	{
		int Price = 0;
		int CurrentGold = OwningHUD->GetPlayerGold();
		if (OwningHUD->GInstance->SpeedUpgradeCounter == 0) { Price = OwningHUD->Tier1Price; }
		else if (OwningHUD->GInstance->SpeedUpgradeCounter == 1) { Price = OwningHUD->Tier2Price; }
		else if (OwningHUD->GInstance->SpeedUpgradeCounter == 2) { Price = OwningHUD->Tier3Price; }
		if (CurrentGold >= Price)
		{
			OwningHUD->AddUpgrade(2);
			OwningHUD->NotificationIndex = 1;
			int NewGoldAmount = CurrentGold -= Price;
			OwningHUD->SetGold(NewGoldAmount);
			OwningHUD->GInstance->SpeedUpgradeCounter++;
		}
		else
		{
			OwningHUD->NotificationIndex = 2;
		}
	}
	return FReply::Handled();
}
/*This will return the player back to the main menu*/
FReply SPerkMenuWidget::ReturnToMenuClicked() const
{
	/*Remove the current menu*/
	OwningHUD->RemoveMenu();
	/*Display the new menu*/
	OwningHUD->ShowMainMenu();
	return FReply::Handled();
}
