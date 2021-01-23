// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

#define LOCTEXT_NAMESPACE "Score Menu"

class SGamePlayWidget : public SCompoundWidget
{
public:
	// The diemensions we want for the cotent on the widget
	const FMargin ContentPadding = FMargin(500.f, 300.f);
	// The Padding for the widgets
	const FMargin ButtonPadding = FMargin(10.0f);

	SLATE_BEGIN_ARGS(SGamePlayWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()
	/*Used to construct the Slate Widget*/
	void Construct(const FArguments& InArgs);
	/*Pointer to the Menu Hud*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };
	/*Gets the current Game Time*/
	FText GetTimer() const;
	/*Gets the current Health*/
	FText GetHealthText() const;
	/*Gest the current Ammo*/
	FText GetAmmoText() const;
};
