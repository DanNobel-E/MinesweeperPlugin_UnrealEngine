// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperButton.h"

void SMinesweeperButton::Construct(const FArguments& InArgs)
{
	SButton::Construct(InArgs);

	ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SAssignNew(TextBlock, STextBlock)
				.Text(FText::FromString(TEXT("")))
				.IsEnabled(false)
				.Visibility(EVisibility::Hidden)
			]
			+SOverlay::Slot()
			[
				SAssignNew(MineImage,SImage)
				.Image(FMinesweeperStyleSet::Get().GetBrush("Minesweeper.MineIcon"))
				.IsEnabled(false)
				.Visibility(EVisibility::Hidden)
			]
		];

	HasMine = false;
	AdjacentMinesCounter = 0;

}


void SMinesweeperButton::EnableTextBlock(bool Enable, const FText& InText)
{

	TextBlock->SetEnabled(Enable);
	TextBlock->SetText(InText);

	if (Enable)
	{

		TextBlock->SetVisibility(EVisibility::All);

	}
	else
	{
		TextBlock->SetVisibility(EVisibility::Hidden);

	}



}

void SMinesweeperButton::EnableImage(bool Enable)
{

	MineImage->SetEnabled(Enable);

	if (Enable)
	{

		MineImage->SetVisibility(EVisibility::All);

	}
	else
	{
		MineImage->SetVisibility(EVisibility::Hidden);

	}


}


