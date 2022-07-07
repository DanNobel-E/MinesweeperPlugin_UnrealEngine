// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperLevel.h"

FMinesweeperLevel::FMinesweeperLevel(const int32 InRows, const int32 InCols, const int32 InMines, const float InCellWidth, const float InCellHeight)
{
	GridRows = InRows;
	GridCols = InCols;
	Mines = InMines;

	CellWidth = InCellWidth;
	CellHeight = InCellHeight;

	InstructionsMessage = TEXT("Welcome to Minesweeper!\n\n\
How to play:\n\
- Fill settings fields and click on \"Generate Level\" button.\n\
- Pick a random cell on the generated level.\n\
- Hope there's not a Mine hidden below!\n\n\n");

	WinMessage = TEXT("You WIN!");
	LoseMessage = TEXT("You LOSE...");


}


FMinesweeperLevel::~FMinesweeperLevel()
{


}

void FMinesweeperLevel::ChangeSize(const int32 InRows, const int32 InCols)
{
	GridRows = InRows;
	GridCols = InCols;

}


void FMinesweeperLevel::ChangeMines(const int32 InMines)
{
	Mines = InMines;

}


int32 FMinesweeperLevel::GetLevelRows() const
{
	return GridRows;

}

int32 FMinesweeperLevel::GetLevelCols() const
{
	return GridCols;

}


int32 FMinesweeperLevel::GetLevelSize() const
{
	return GridRows * GridCols;

}

void FMinesweeperLevel::ResetLevel(TSharedRef<SWrapBox> InGameBox, const int32 InSize, const bool UpSize)
{

	if (UpSize)
	{

		int32 SizeDelta = InSize - LevelSlots.Num();
		SlotButtons.SetNum(SlotButtons.Num() + SizeDelta);
		SlotWidgets.SetNum(SlotWidgets.Num() + SizeDelta);
		LevelSlots.SetNum(LevelSlots.Num() + SizeDelta);
		for (int32 It = LevelSlots.Num() - SizeDelta; It < LevelSlots.Num(); It++)
		{
			auto OnClickedLambda = [this, It]()
			{
				ClickButton(SlotButtons[It].ToSharedRef(), It);
				return FReply::Handled();
			};

			InGameBox->AddSlot()
				.Expose(LevelSlots[It])
				[

					SAssignNew(SlotWidgets[It], SBox)
					.WidthOverride(CellWidth)
					.HeightOverride(CellHeight)
					[

						SAssignNew(SlotButtons[It], SMinesweeperButton)
						.OnClicked(FOnClicked::CreateLambda(OnClickedLambda))
						.ButtonColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.ButtonColor")))


					]

				];

		}

	}
	else
	{
		for (int32 It = LevelSlots.Num() - 1; It >= InSize; It--)
		{
			InGameBox->RemoveSlot(SlotWidgets[It].ToSharedRef());
		}


		SlotButtons.SetNum(InSize);
		SlotWidgets.SetNum(InSize);
		LevelSlots.SetNum(InSize);


	}

	ResetButtons();
	ResetMinesIndexes();
	AssignMinesIndexes();
	AvailableCellsCounter = GetLevelSize();

	InGameBox->SetWrapSize(CellWidth * GridCols);


}

void FMinesweeperLevel::ResetButtons()
{
	for (TSharedPtr<SMinesweeperButton> Button : SlotButtons)
	{
		Button->SetEnabled(true);
		Button->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.ButtonColor")));
		Button->HasMine = false;
		Button->AdjacentMinesCounter = 0;

		Button->EnableTextBlock(false, FText::FromString(TEXT("")));
		Button->EnableImage(false);

	}
}

void FMinesweeperLevel::ResetMinesIndexes()
{

	MinesIndexes.Empty(Mines);

	int32 Size = GetLevelSize();
	int32 NewIndex = -1;

	for (int32 It = 0; It < Mines; It++)
	{
		do
		{

			NewIndex = FMath::RandRange(0, Size - 1);

		} while (MinesIndexes.Contains(NewIndex));

		MinesIndexes.Add(NewIndex);


	}


}

void FMinesweeperLevel::AssignMinesIndexes()
{

	if (MinesIndexes.Num() > 0)
	{
		int32 Size = GetLevelSize();

		for (int32 Index : MinesIndexes)
		{
			if (Index < Size)
			{

				int32 Row = Index / GridCols;
				int32 Col = Index % GridCols;

				SlotButtons[Index]->HasMine = true;

				if (Row + 1 < GridRows)
				{
					GetButtonAtIndex(Row + 1, Col)->AdjacentMinesCounter++;

				}

				if (Row - 1 >= 0)
				{
					GetButtonAtIndex(Row - 1, Col)->AdjacentMinesCounter++;

				}

				if (Col + 1 < GridCols)
				{
					GetButtonAtIndex(Row, Col + 1)->AdjacentMinesCounter++;

				}

				if (Col - 1 >= 0)
				{
					GetButtonAtIndex(Row, Col - 1)->AdjacentMinesCounter++;

				}

			}


		}


	}

}

TSharedPtr<SMinesweeperButton> FMinesweeperLevel::GetButtonAtIndex(const int32 InRow, const int32 InCol) const
{
	int32 Index = InRow * GridCols + InCol;

	if (Index < 0 || Index >= SlotButtons.Num())
	{
		return nullptr;
	}

	return SlotButtons[Index];

}


void FMinesweeperLevel::ClickButton(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
{
	if (InButton.IsValid())
	{
		if (InButton->IsEnabled())
		{
			if (InButton->HasMine)
			{
				MineButtonClicked(InButton, InIndex);

			}
			else
			{

				FreeButtonClicked(InButton, InIndex);
				if (AvailableCellsCounter <= Mines)
				{
					int32 Size = GetLevelSize();

					for (int32 Index : MinesIndexes)
					{
						if (Index < Size)
						{

							SlotButtons[Index]->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.WinMineColor")));
							SlotButtons[Index]->EnableImage(true);

						}


					}


					OnMinesweeperEnd.ExecuteIfBound(true);

				}
			}
		}
	}

}


void FMinesweeperLevel::MineButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
{
	int32 Size = GetLevelSize();

	InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HasMineColor")));
	InButton->EnableImage(true);

	for (int32 Index : MinesIndexes)
	{
		if (Index != InIndex)
		{

			if (Index < Size)
			{
				SlotButtons[Index]->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HasMineColor")));
				SlotButtons[Index]->EnableImage(true);

			}

		}

	}

	OnMinesweeperEnd.ExecuteIfBound(false);
	

}


void FMinesweeperLevel::FreeButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
{

	InButton->SetEnabled(false);

	if (InButton->AdjacentMinesCounter > 0)
	{
		
		FString Msg = FString::Printf(TEXT("%i"), InButton->AdjacentMinesCounter);
		InButton->EnableTextBlock(true, FText::FromString(Msg));


		FString ColorString = TEXT("Minesweeper.MineColor_") + Msg;
		InButton->SetForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.DisabledColor")));
		InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(*ColorString));


	}
	else
	{


		InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.DisabledColor")));
		
		int32 Row = InIndex / GridCols;
		int32 Col = InIndex % GridCols;

		TSharedPtr<SMinesweeperButton> NeighbourButton = nullptr;
		int32 NeighbourIndex = InIndex;

		if (Row + 1 < GridRows)
		{
			NeighbourIndex = (Row + 1) * GridCols + Col;
			NeighbourButton = GetButtonAtIndex(Row + 1, Col);
			ClickButton(NeighbourButton, NeighbourIndex);


		}

		if (Row - 1 >= 0)
		{
			NeighbourIndex = (Row - 1) * GridCols + Col;
			NeighbourButton = GetButtonAtIndex(Row - 1, Col);
			ClickButton(NeighbourButton, NeighbourIndex);

		}

		if (Col + 1 < GridCols)
		{
			NeighbourIndex = Row * GridCols + (Col + 1);
			NeighbourButton = GetButtonAtIndex(Row, Col + 1);
			ClickButton(NeighbourButton, NeighbourIndex);

		}

		if (Col - 1 >= 0)
		{
			NeighbourIndex = Row * GridCols + (Col - 1);
			NeighbourButton = GetButtonAtIndex(Row, Col - 1);
			ClickButton(NeighbourButton, NeighbourIndex);

		}


	}

	AvailableCellsCounter--;

}

