// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperStyleSet.h"


TSharedPtr< ISlateStyle > FMinesweeperStyleSet::StyleSet = NULL;


FMinesweeperStyleSet::FMinesweeperStyleSet(const FName& InName) : FSlateStyleSet(InName)
{

}

FMinesweeperStyleSet::~FMinesweeperStyleSet()
{
	
}


void FMinesweeperStyleSet::Initialize()
{
	if (StyleSet.IsValid())
	{
		return;
	}
	StyleSet = Create();
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

TSharedRef< FSlateStyleSet > FMinesweeperStyleSet::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShared<FSlateStyleSet>(TEXT("MinesweeperStyle"));
	FString RootPath = IPluginManager::Get().FindPlugin("MinesweeperPlugin")->GetBaseDir() + TEXT("/Resources");
	Style->SetContentRoot(RootPath);
	FString IconPath = RootPath + TEXT("/MS.png");
	FSlateImageBrush* IconBrush48 = new FSlateImageBrush(IconPath, FVector2D(48.f,48.f));
	FSlateImageBrush* IconBrush32 = new FSlateImageBrush(IconPath, FVector2D(32.f, 32.f));
	FSlateImageBrush* IconBrush16 = new FSlateImageBrush(IconPath, FVector2D(16.f, 16.f));
	FString GameIconPath = RootPath + TEXT("/MSLogo.png");
	FSlateImageBrush* GameIconBrush = new FSlateImageBrush(GameIconPath, FVector2D(16.f, 16.f));
	FSlateImageBrush* MineIconBrush = new FSlateImageBrush(GameIconPath, FVector2D(8.f, 8.f));

	FString SettingsIconPath = RootPath + TEXT("/MSSettings.jpg");
	FSlateImageBrush* SettingsIconBrush = new FSlateImageBrush(SettingsIconPath, FVector2D(16.f, 16.f));
	Style->Set("Minesweeper.Icon48", IconBrush48);
	Style->Set("Minesweeper.Icon32", IconBrush32);
	Style->Set("Minesweeper.Icon16", IconBrush16);
	Style->Set("Minesweeper.GameIcon", GameIconBrush);
	Style->Set("Minesweeper.MineIcon", MineIconBrush);

	Style->Set("Minesweeper.SettingsIcon", SettingsIconBrush);

	FSlateColor TextColor = FLinearColor(1.f, 1.f, 1.f);
	Style->Set("Minesweeper.TextColor", TextColor);

	FSlateColor BorderColor = FLinearColor(1.f, 1.f, 1.f);
	Style->Set("Minesweeper.BorderColor", BorderColor);

	FSlateColor ButtonColor = FLinearColor(1.f, 1.f, 1.f);
	Style->Set("Minesweeper.ButtonColor", ButtonColor);

	FSlateColor MainColor1 = FLinearColor(0.8f, 0.8f, 0.8f);
	Style->Set("Minesweeper.MainColor1", MainColor1);

	FSlateColor MainColor2 = FLinearColor(0.1f, 0.1f, 0.1f);
	Style->Set("Minesweeper.MainColor2", MainColor2);

	FSlateColor MainColor3 = FLinearColor(0.3f, 0.3f, 0.3f);
	Style->Set("Minesweeper.MainColor3", MainColor3);

	FSlateColor EditColor = FLinearColor(0.05f, 0.05f, 0.05f);
	Style->Set("Minesweeper.EditColor", EditColor);

	FSlateColor HighlightColor = FLinearColor(1.f, 1.f, 0.f);
	Style->Set("Minesweeper.HighlightColor", HighlightColor);

	FSlateColor DisabledColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
	Style->Set("Minesweeper.DisabledColor", DisabledColor);

	FSlateColor MineColor1 = FLinearColor(0.f, 1.f, 0.f, 1.f);
	Style->Set("Minesweeper.MineColor_1", MineColor1);
	FSlateColor MineColor2 = FLinearColor(1.f, 1.f, 0.f, 1.f);
	Style->Set("Minesweeper.MineColor_2", MineColor2);
	FSlateColor MineColor3 = FLinearColor(1.f, 0.5f, 0.f, 1.f);
	Style->Set("Minesweeper.MineColor_3", MineColor3);
	FSlateColor MineColor4 = FLinearColor(1.f, 0.f, 0.f, 1.f);
	Style->Set("Minesweeper.MineColor_4", MineColor4);


	FSlateColor HasMineColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
	Style->Set("Minesweeper.HasMineColor", HasMineColor);

	FSlateColor WinMineColor = FLinearColor(0.f, 1.f, 0.f, 1.f);
	Style->Set("Minesweeper.WinMineColor", WinMineColor);

	FMargin TabMargin = FMargin(10.f, 10.f);
	Style->Set("Minesweeper.TabMargin", TabMargin);

	return Style;
}

ISlateStyle& FMinesweeperStyleSet::Get()
{
	return *(StyleSet.Get());
}

void FMinesweeperStyleSet::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

const FName& FMinesweeperStyleSet::GetStyleSetName() const
{
	return StyleSetName;
}
