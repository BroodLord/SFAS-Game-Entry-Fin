#include "SGamePlayWidget.h"
#include "MenuHUD.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <sstream>
#include <fstream>
#include "Widgets/Input/SEditableTextBox.h"

void SGamePlayWidget::Construct(const FArguments& InArgs)
{

	/*Sets the focus for the menu to be true*/
	bCanSupportFocus = true;
	/*Assigns the owningHUD with the MenuHUD*/
	OwningHUD = InArgs._OwningHUD;
	/*Gets The current game instance*/
	OwningHUD->SetInstance();
	/*This will set up the Title text and font*/
	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.f;

	/*The Title Text setup*/
	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 38.f;

	ChildSlot
		[
			SNew(SOverlay) + SOverlay::Slot().Padding(ButtonPadding).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				// Sets up a wrapbox so they all appear under each other
				SNew(SWrapBox) + SWrapBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
					// Health Text
					SNew(SVerticalBox) + SVerticalBox::Slot()
					[
						SNew(STextBlock).Font(TitleTextStyle).Text(this, &SGamePlayWidget::GetHealthText).Justification(ETextJustify::Center)
					]
				]
				+ SWrapBox::Slot()
					[
						// Ammo Text
					SNew(SVerticalBox) + SVerticalBox::Slot()
						[
							SNew(STextBlock).Font(TitleTextStyle).Text(this, &SGamePlayWidget::GetAmmoText).Justification(ETextJustify::Center)
						]
					]
				+ SWrapBox::Slot()
				[
					// Timer Text
					SNew(STextBlock).Font(TitleTextStyle).Text(this, &SGamePlayWidget::GetTimer).Justification(ETextJustify::Center)
				]
			]
		];

}

/*This will Get the current timer and set the text to that float*/
FText SGamePlayWidget::GetTimer() const
{
	OwningHUD->SetTimeVar();
	FString TempString = "Timer: " + FString::SanitizeFloat(OwningHUD->Time);
	return FText::FromString(TempString);
}
// Gets the current Health and set the text to that int
FText SGamePlayWidget::GetHealthText() const
{
	FString TempString = "Health: " + FString::FromInt(OwningHUD->GetPlayerHealth());
	return FText::FromString(TempString);
}
// Gets the current ammo and set the text to that int
FText SGamePlayWidget::GetAmmoText() const
{
	FString TempString = "Ammo: " + FString::FromInt(OwningHUD->GetPlayerAmmo());
	return FText::FromString(TempString);
}
