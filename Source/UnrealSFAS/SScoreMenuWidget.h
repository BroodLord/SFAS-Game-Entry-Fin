// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

#define LOCTEXT_NAMESPACE "Score Menu"

/**
 * 
 */
class SScoreMenuWidget : public SCompoundWidget
{
public:
	// The diemensions we want for the cotent on the widget
	const FMargin ContentPadding = FMargin(500.f, 300.f);
	// The Padding for the widgets
	const FMargin WidgetPadding = FMargin(10.0f);
	
	// Title Text
	const FText TitleText = LOCTEXT("EndLevelText", "End Of Level!");
	// Quit Text
	const FText QuitText = LOCTEXT("QuitText", "Return to MainMenu");
	/*Texts used to store strings for their assigned widgets*/
	FText KillsText;
	FText TimeText;
	FText ScoreText;
	
	SLATE_BEGIN_ARGS(SScoreMenuWidget) {}
	
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)
	
	SLATE_END_ARGS()
	
	/*Used to construct the Slate Widget*/
	void Construct(const FArguments& InArgs);
	/*Returns the player to the main menu*/
	FReply ReturnToMenuClicked() const;
	/*Assigns the text before they are full assigned in the slate construction*/
	void AssignText();
	/*Pointer to the MenuHUD*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	virtual bool SupportsKeyboardFocus() const override { return true; };
};

#undef LOCTEXT_NAMESPACE
