// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

FMinesweeperCommands::FMinesweeperCommands() : TCommands<FMinesweeperCommands>(
	TEXT("Minesweeper"), NSLOCTEXT("Contexts","MinesweeperPlugin","MinesweeperPlugin Plugin"), NAME_None, 
	FMinesweeperStyleSet::Get().GetStyleSetName()
	)
{

}

FMinesweeperCommands::~FMinesweeperCommands()
{

}


void FMinesweeperCommands::RegisterCommands()
{

	UI_COMMAND(MinesweeperCommand, "Minesweeper", "Open Minesweeper Editor Minigame", EUserInterfaceActionType::Button, FInputGesture());


}

#undef LOCTEXT_NAMESPACE