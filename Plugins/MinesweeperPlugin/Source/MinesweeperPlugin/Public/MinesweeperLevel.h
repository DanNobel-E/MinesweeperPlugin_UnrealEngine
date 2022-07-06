// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperButton.h"
#include "MinesweeperStyleSet.h"
#include "Widgets/Layout/SWrapBox.h"

DECLARE_DELEGATE_OneParam(FOnMinesweeperEnd, bool)

class MINESWEEPERPLUGIN_API FMinesweeperLevel
{
public:
public:

	FMinesweeperLevel(const int32 InRows, const int32 InCols, const int32 InMines, const float InCellWidth, const float InCellHeight);
	~FMinesweeperLevel();

	void ChangeSize(const int32 InRows, const int32 InCols);
	void ChangeMines(const int32 InMines);

	int32 GetLevelRows() const;
	int32 GetLevelCols() const;
	int32 GetLevelSize() const;

	void ResetLevel(TSharedRef<SWrapBox> InGameBox, const int32 InSize, const bool UpSize);
	void ResetButtons();
	void ResetMinesIndexes();
	void AssignMinesIndexes();
	TSharedPtr<SMinesweeperButton> GetButtonAtIndex(const int32 InRow, const int32 InCol) const;

	void ClickButton(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex);
	void MineButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex);
	void FreeButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex);

	int32 CellWidth;
	int32 CellHeight;
	int32 AvailableCellsCounter;

	FString InstructionsMessage;
	FString WinMessage;
	FString LoseMessage;


protected:

	int32 GridRows;
	int32 GridCols;

	int32 Mines;

public:
	TArray<SWrapBox::FSlot*> LevelSlots;
	TArray<TSharedPtr<SWidget>> SlotWidgets;
	TArray<TSharedPtr<SMinesweeperButton>> SlotButtons;

	TSet<int32> MinesIndexes;

	FOnMinesweeperEnd OnMinesweeperEnd;
};
