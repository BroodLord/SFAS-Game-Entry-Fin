// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

#define LOCTEXT_NAMESPACE "Main Menu"

/**
 * 
 */
class  SHowToPlayMenuWidget : public SCompoundWidget
{
public:
	// The diemensions we want for the cotent on the widget
	const FMargin ContentPadding = FMargin(100.f, 10.f);
	// The Padding for the widgets
	const FMargin ButtonPadding = FMargin(20.0f);

	/*Title Text*/
	const FText TitleText = LOCTEXT("GameTitle", "How To Play");
	/*Texts that show the user how to play the game*/
	const FText Text1 = LOCTEXT("Text1", "Welcome to my game, In this game you are spawned into a maze and have to find your Toblerone,");
	const FText Text2 = LOCTEXT("Text2", "Zombies will spawn and try and stop you though so be very careful");
	const FText Text3 = LOCTEXT("Text3", "You will gain points when you kill zombies!");
	const FText Text4 = LOCTEXT("Text4", "When you complete the level you will gain gold");
	const FText Text5 = LOCTEXT("Text5", "which can be spent on perks but if you die you lose all your perks. ");
	const FText Text6 = LOCTEXT("Text6", "Each level takes a 4 letter combination to generate a level or you");
	const FText Text7 = LOCTEXT("Text7", "can make your own levels and use them, see levels file for more");
	const FText Text8 = LOCTEXT("Text8", "Thanks for downloading and playing my game! I hope you enjoy <3");
	const FText Text9 = LOCTEXT("Text9", "Made by Danny Kavanagh, Github - https://github.com/BroodLord");
	const FText Text10 = LOCTEXT("Text10", "WASD For Movement, Space to Shoot, Mouse for aiming");
	/***********************************************************************************************************/
	/*Return to Menu Text*/
	const FText ReturnText = LOCTEXT("ReturnToMenu", "Return To Menu");

	SLATE_BEGIN_ARGS(SHowToPlayMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	/*Used to construct the Slate Widget*/
	void Construct(const FArguments& InArgs);
	/*Used to return to the Main Menu*/
	FReply ReturnToMenuClicked() const;
	/*Pointer to the Menu Hud*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	virtual bool SupportsKeyboardFocus() const override { return true; };
};

#undef LOCTEXT_NAMESPACE
