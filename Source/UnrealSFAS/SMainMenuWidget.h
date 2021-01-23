// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

#define LOCTEXT_NAMESPACE "Main Menu"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	// The diemensions we want for the cotent on the widget
	const FMargin ContentPadding = FMargin(300.f, 100.f);
	// The Padding for the widgets
	const FMargin ButtonPadding = FMargin(5.0f);
	// Title Text
	const FText TitleText = LOCTEXT("GameTitle", "TOBLERONE HUNTER");
	// Play Text
	const FText PlayText = LOCTEXT("PlayGame", "Play");
	// Quit Text
	const FText QuitText = LOCTEXT("QuitGame", "QuitGame");
	// How To Play Menu Text
	const FText PlayMenuText = LOCTEXT("How To Play", "How To Play");
	// Perk Menu Text
	const FText PerkMenuText = LOCTEXT("PerkMenu", "PerkMenu");
	// Notification Title Text
	const FText ConstNotificationText = LOCTEXT("NotificationText", "Notification Box");
	/*Notifcation Texts*/
	const FString NotificationOne = "LEVEL ID DOESN'T = 4/ LEVEL NAME INVALID";
	const FString NotificationTwo = "TEXT FIELD IS EMPTY";
	const FString NotificationThree = "SEED ID CONTAINS NONE LETTER";
	/**********************************************/

	const int MAX_SEED_LENGTH = 4;

	/*Stores the Notification Texts, Stored in an array to get rid of code duplication*/
	FString NotificationArray[4] = { "", NotificationOne, NotificationTwo,NotificationThree };

	/*Text used to store the seed/level name*/
	FString SeedText;

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	/*Used to construct the Slate Widget*/
	void Construct(const FArguments& InArgs);
	/*Pointer to the Menu Hud*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	// Used to update the seed Text when the text is changed
	void OnTextChanged(const FText& InText, ETextCommit::Type);
	// Loads the playable level
	FReply OnPlayClicked() const;
	/*Loads the Perk Menu Widget*/
	FReply OnPerkMenuClicked() const;
	/*Loads the How To Play Menu*/
	FReply OnPlayMenuClicked() const;
	/*Used to close the program*/
	FReply OnQuitClicked() const;
	/*Get the current Gold Text*/
	FText GetGoldText() const;
	/*Gets the current Notification*/
	FText GetCurrentNotificationText() const;
	/*Gets the edited text*/
	FText GetEditedText() const;

	virtual bool SupportsKeyboardFocus() const override { return true; };
};

#undef LOCTEXT_NAMESPACE
