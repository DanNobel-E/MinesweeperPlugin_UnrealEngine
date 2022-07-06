// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperStyleSet.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SOverlay.h"


class MINESWEEPERPLUGIN_API SMinesweeperButton : public SButton
{

public:

	void Construct(const FArguments& InArgs);
	void EnableTextBlock(bool Enable, const FText& InText);
	void EnableImage(bool Enable);

	bool HasMine;
	int32 AdjacentMinesCounter;

	TSharedPtr<STextBlock> TextBlock;
	TSharedPtr<SImage> MineImage;



};
