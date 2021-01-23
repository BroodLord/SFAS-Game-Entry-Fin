// Fill out your copyright notice in the Description page of Project Settings.


#include "SHowToPlayMenuWidget.h"
#include "MenuHUD.h"

void SHowToPlayMenuWidget::Construct(const FArguments& InArgs)
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
	TitleTextStyle.Size = 60.f;

	
	/*Main Body Text*/
	FSlateFontInfo MainTextStyle = ButtonTextStyle;
	MainTextStyle.Size = 20.f;


	ChildSlot
		[
			SNew(SOverlay) + SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
			[

				//Title Text
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(TitleTextStyle).Text(TitleText).Justification(ETextJustify::Center)
				]
	/*Displays all the text outputs for how to play the game*/
			+ SVerticalBox::Slot()
			[
			SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text1).Justification(ETextJustify::Center)
				]
			
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text2).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text3).Justification(ETextJustify::Center)
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text4).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text5).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text6).Justification(ETextJustify::Center)
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text7).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text10).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text8).Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
				[
					SNew(STextBlock).Font(MainTextStyle).Text(Text9).Justification(ETextJustify::Center)
				]
			]
		/****************************************************************************/
			+ SVerticalBox::Slot().Padding(100,100)
				[
					/*Quit Button*/
					SNew(SButton).OnClicked(this, &SHowToPlayMenuWidget::ReturnToMenuClicked)
					[
						SNew(STextBlock).Font(TitleTextStyle).Text(ReturnText).Justification(ETextJustify::Center)
					]
				]
			]
		];
}
/*This will remove the current menu and show the main ment*/
FReply SHowToPlayMenuWidget::ReturnToMenuClicked() const
{
	OwningHUD->RemoveMenu();
	OwningHUD->ShowMainMenu();
	return FReply::Handled();
}
