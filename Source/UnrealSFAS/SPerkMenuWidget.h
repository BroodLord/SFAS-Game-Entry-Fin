// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

#define LOCTEXT_NAMESPACE "Score Menu"

/**
 * 
 */
class SPerkMenuWidget : public SCompoundWidget
{
public:

	// The diemensions we want for the cotent on the widget
	const FMargin ContentPadding = FMargin(300.f, 100.f);
	// The Padding for the widgets
	const FMargin WidgetPadding = FMargin(5.0f);

	// Title Text
	const FText TitleText = LOCTEXT("PerkTitle", "PerkMenu");
	// Notification Text Title
	const FText ConstNotificationText = LOCTEXT("NotificationText", "Notification Box");
	// Quit Text
	const FText QuitText = LOCTEXT("QuitText", "Return to MainMenu");
	/*This text will display the prices and the matching tier for them*/
	const FString UpgradeTier1 = "Upgrade Level Tier 1 (500 Gold)";
	const FString UpgradeTier2 = "Upgrade Level Tier 2 (800 Gold)";
	const FString UpgradeTier3 = "Upgrade Level Tier 3 (1200 Gold)";
	const FString UpgradeMAX = "Upgrade Level: MAX";
	/**************************************************************/
	/*Notification Texts*/
	const FString NotificationOne = "PERK UPGRADED!";
	const FString NotificationTwo = "YOU CAN'T AFFORD THIS PERK UPGRADE";
	/*********************************/
	/*The various upgrade texts for the different perks*/
	const FString UpgradeDamageText = "Damage Upgrade!";
	const FString UpgradeAmmoText = "Ammo Upgrade! ";
	const FString UpgradeSpeedText = "Speed Upgrade! ";
	/***************************************************/

	/*Stores the Notification Texts, Stored in an array to get rid of code duplication*/
	FString NotificationArray[3] = {"", NotificationOne, NotificationTwo };

	SLATE_BEGIN_ARGS(SPerkMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	/*Used to construct the Slate Widget*/
	void Construct(const FArguments& InArgs);
	/*Gets the text slots for the different Text fields*/
	FText GetSlot1Text() const;
	FText GetSlot2Text() const;
	FText GetSlot3Text() const;
	/***************************************************/
	FText GetGoldText() const; // Gets the text for the player gold
	/*These function will attempt to upgrade the assigned perk*/
	FReply UpgradeDamage() const;
	FReply UpgradeAmmo() const;
	FReply UpgradeSpeed() const;
	/*Gets the current Notification Text*/
	FText GetCurrentNotificationText() const;
	/*Returns the player back to the main menu*/
	FReply ReturnToMenuClicked() const;
	/*Pointer to the menu hud*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	virtual bool SupportsKeyboardFocus() const override { return true; };
};
