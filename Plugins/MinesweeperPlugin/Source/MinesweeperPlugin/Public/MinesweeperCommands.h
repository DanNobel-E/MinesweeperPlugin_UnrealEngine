// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "MinesweeperStyleSet.h"
#include "SlateBasics.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

/**
 * 
 */
class MINESWEEPERPLUGIN_API FMinesweeperCommands : public TCommands<FMinesweeperCommands>
{
public:
	FMinesweeperCommands();
	~FMinesweeperCommands();

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> MinesweeperCommand;
};
