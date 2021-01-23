// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainMenuWidget.h"
#include "MenuHUD.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <sstream>
#include <fstream>
#include "Widgets/Input/SEditableTextBox.h"



void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	/*Sets the seed text to default*/
	SeedText = "ENTER SEED/LEVELNAME";

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
	FSlateFontInfo NotifTextStyle = ButtonTextStyle;
	NotifTextStyle.Size = 40.f;

	ChildSlot
		[
			// Gold Text
	        SNew(SOverlay) + SOverlay::Slot()
				[
				 SNew(STextBlock).Font(TitleTextStyle).Text(this, &SMainMenuWidget::GetGoldText).Justification(ETextJustify::Right)
				]
			+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
			[

				//Title Text
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(TitleTextStyle).Text(TitleText).Justification(ETextJustify::Center)
				]

				// Play Button
				+ SVerticalBox::Slot().Padding(ButtonPadding)
				[
					SNew(SButton).OnClicked(this, &SMainMenuWidget::OnPlayClicked)
					[
						SNew(STextBlock).Font(ButtonTextStyle).Text(PlayText).Justification(ETextJustify::Center)
					]
				]
				// Seed Field
				+ SVerticalBox::Slot().Padding(ButtonPadding)
					[
						SNew(SEditableTextBox).Font(ButtonTextStyle).Text(this, &SMainMenuWidget::GetEditedText).Justification(ETextJustify::Center).OnTextCommitted(this, &SMainMenuWidget::OnTextChanged)
					]
				// Perk Field
				+ SVerticalBox::Slot().Padding(ButtonPadding)
					[
						SNew(SButton).OnClicked(this, &SMainMenuWidget::OnPerkMenuClicked)
						[
							SNew(STextBlock).Font(ButtonTextStyle).Text(PerkMenuText).Justification(ETextJustify::Center)
						]
					]
				// How To Play Field
				+ SVerticalBox::Slot().Padding(ButtonPadding)
					[
						SNew(SButton).OnClicked(this, &SMainMenuWidget::OnPlayMenuClicked)
						[
							SNew(STextBlock).Font(ButtonTextStyle).Text(PlayMenuText).Justification(ETextJustify::Center)
						]
					]
				// Quit Button
				+ SVerticalBox::Slot().Padding(ButtonPadding)
				[
					SNew(SButton).OnClicked(this, &SMainMenuWidget::OnQuitClicked)
					[
						SNew(STextBlock).Font(ButtonTextStyle).Text(QuitText).Justification(ETextJustify::Center)
					]
				]
				// Notification Title Text
				+ SVerticalBox::Slot().Padding(ButtonPadding)
					[
						SNew(STextBlock).Font(TitleTextStyle).Text(ConstNotificationText).Justification(ETextJustify::Center)
					]
				// Notification Text
				+ SVerticalBox::Slot().Padding(ButtonPadding)
					[
						SNew(SButton)
						[
							SNew(STextBlock).Font(NotifTextStyle).Text(this, &SMainMenuWidget::GetCurrentNotificationText).Justification(ETextJustify::Center)
						]
					]

			]
		];
}

// Gets the changed text field and sets it to the seed text
void SMainMenuWidget::OnTextChanged(const FText& InText, ETextCommit::Type)
{
	FString EqualFText = InText.ToString();
	SeedText = EqualFText;
}

// Will remove the current menu and load the perk menu
FReply SMainMenuWidget::OnPerkMenuClicked() const
{
	OwningHUD->RemoveMenu();
	OwningHUD->ShowPerkMenu();
	return FReply::Handled();
}
// Will remove the current menu and load the play menu
FReply SMainMenuWidget::OnPlayMenuClicked() const
{
	OwningHUD->RemoveMenu();
	OwningHUD->ShowHowToPlayMenu();
	return FReply::Handled();
}
// This will set up and load the player able seen
FReply SMainMenuWidget::OnPlayClicked() const
{
	if (OwningHUD.IsValid())
	{
		// Gets the seed text and convert it to std::string
		std::string NewTest = std::string(TCHAR_TO_UTF8(*SeedText));
		// Gets the full path for the project
		FString FullPath = FPaths::ProjectDir();
		// This line will get the Path, add the fill we want to check and add the level name to get the path we want to check
		std::string TempPath = std::string(TCHAR_TO_UTF8(*FullPath)) + "\\Levels\\" + NewTest;
		std::ifstream InFile;
		// Will atempt to open the file path
		InFile.open(TempPath);
		if (!SeedText.IsEmpty()) // If the seed isn't empty
		{
				// If we can open that file or the seed is the current length
				if (InFile.is_open() || SeedText.Len() == MAX_SEED_LENGTH)
				{
					/*Will convert the current text to lowercase*/
					std::string LowerCase = NewTest;
					for (int i = 0; i < NewTest.length(); i++)
					{
						LowerCase[i] = tolower(NewTest[i]);
					}
					/**********************************************/
					// Check again to max sure that the Seed is either greater then 4 (Is a levelName) or check if the seed is valid.
					if (SeedText.Len() > MAX_SEED_LENGTH || OwningHUD->CheckSeedIsValid(LowerCase))
					{
						/*Set the seed, remove the menu and load the level*/
						OwningHUD->SetSeed(LowerCase);
						OwningHUD->RemoveMenu();
						OwningHUD->LoadLevel("MazeBase");
						/***************************************/
					}
				}
				/*Display notification 1*/
				else { OwningHUD->NotificationIndex = 1; }
		}
		/*Display notification 2*/
		else { OwningHUD->NotificationIndex = 2; }
	}
	return FReply::Handled();
}
// Will quit the program
FReply SMainMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("Quit");
		}
	}

	return FReply::Handled();
}
// Sets the editedtext field to be the seed text
FText SMainMenuWidget::GetEditedText() const
{
	return FText::FromString(SeedText);
}
// Will return the current notification text
FText SMainMenuWidget::GetCurrentNotificationText() const
{
	/*Depending on the Notification Index display the assigned Notification*/
	for (int i = 0; i < 4; i++)
	{
		if (OwningHUD->NotificationIndex == i)
		{
			FString sTemp = NotificationArray[i];
			FText fTemp = FText::FromString(sTemp);
			return fTemp;
		}
	}

	//if(OwningHUD->NotificationIndex == 1)
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
	//else if (OwningHUD->NotificationIndex == 3)
	//{
	//	FString sTemp = NotificationThree;
	//	FText fTemp = FText::FromString(sTemp);
	//	return fTemp;
	//}
	return FText();
}
// Gets the current gold text
FText SMainMenuWidget::GetGoldText() const
{
	FText ReturnText = FText::FromString("GOLD: " + FString::FromInt(OwningHUD->GetPlayerGold()));
	return ReturnText;
}
