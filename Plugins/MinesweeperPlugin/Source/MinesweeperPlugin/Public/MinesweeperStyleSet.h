// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageUtils.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

/**
 * 
 */
class MINESWEEPERPLUGIN_API FMinesweeperStyleSet : public FSlateStyleSet
{
public:
	FMinesweeperStyleSet(const FName& InName);
	~FMinesweeperStyleSet();

	static void Initialize();

	static TSharedRef<FSlateStyleSet> Create();

	static ISlateStyle& Get();

	static void Shutdown();
	
	static TSharedPtr<class ISlateStyle> StyleSet;

	virtual const FName& GetStyleSetName() const override;


};
