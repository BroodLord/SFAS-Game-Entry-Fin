// Fill out your copyright notice in the Description page of Project Settings.


#include "SScoreMenuWidget.h"
#include "MenuHUD.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <sstream>
#include <fstream>
#include "Widgets/Input/SEditableTextBox.h"



void SScoreMenuWidget::Construct(const FArguments& InArgs)
{
	/*Sets the focus for the menu to be true*/
	bCanSupportFocus = true;
	/*Assigns the owningHUD with the MenuHUD*/
	OwningHUD = InArgs._OwningHUD;
	/*Assigns Text*/
	AssignText();
	
	/*This will set up the text and font*/
	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.f;
	
	/*The Title Text setup*/
	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.f;
	
	ChildSlot
		[
			/*Creates a black background for the UI*/
			SNew(SOverlay) + SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SImage).ColorAndOpacity(FColor::Black)
			]
		/*Creates a new slot*/
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
			[
	
			    //Title Text
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(TitleTextStyle).Text(TitleText).Justification(ETextJustify::Center)
				]
	
				//Kills Text
				+ SVerticalBox::Slot().Padding(WidgetPadding)
				[
					SNew(STextBlock).Font(TitleTextStyle).Justification(ETextJustify::Center).Text(KillsText)
				]
				//Time Text
				+ SVerticalBox::Slot().Padding(WidgetPadding)
					[
						SNew(STextBlock).Font(TitleTextStyle).Justification(ETextJustify::Center).Text(TimeText)
					]
				//Score Text
				+ SVerticalBox::Slot().Padding(WidgetPadding)
					[
						SNew(STextBlock).Font(TitleTextStyle).Justification(ETextJustify::Center).Text(ScoreText)
					]
				// Quit Button
				+SVerticalBox::Slot().Padding(WidgetPadding)
				[
					// When the button is pressed it will return the player back to the menu
					SNew(SButton).OnClicked(this, &SScoreMenuWidget::ReturnToMenuClicked)
					[
						SNew(STextBlock).Font(TitleTextStyle).Text(QuitText).Justification(ETextJustify::Center)
					]
				]
				
			]
		];
		
}

/*This function will load the selected level if the MenuHud is Valid*/
FReply SScoreMenuWidget::ReturnToMenuClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->LoadLevel("MainMenu");
	}

	return FReply::Handled();
}

/*This will assign the texts of the Kills, Times and Score for the UI*/
void SScoreMenuWidget::AssignText()
{

	FString sKillsText = "Kills: " + OwningHUD->SetKillText();
	FString sTimeText = "Time: " + OwningHUD->SetTimeText();
	FString sScoreText = "Score: " + OwningHUD->SetScoreText();
	FText tempKillsText = FText::FromString(sKillsText);
	FText tempTimeText = FText::FromString(sTimeText);
	FText tempScoreText = FText::FromString(sScoreText);
	KillsText = tempKillsText;
	TimeText = tempTimeText;
	ScoreText = tempScoreText;
}
