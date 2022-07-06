// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Input/Reply.h"
#include "LevelEditor.h"
#include "MinesweeperButton.h"
#include "MinesweeperCommands.h"
#include "MinesweeperLevel.h"
#include "MinesweeperStyleSet.h"
#include "Misc/CString.h"
#include "Modules/ModuleManager.h"
#include "Types/SlateStructs.h"
#include "Widgets/Colors/SSimpleGradient.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Docking/SDockTabStack.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

class FMinesweeperPluginModule : public IModuleInterface, public TSharedFromThis<FMinesweeperPluginModule>
{
public:


	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void OpenMinesweeperWindow();
	virtual void AddToolbarButton(FToolBarBuilder& Builder);

	TSharedRef<SDockTab> SpawnMainTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnSettingsTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnGameTab(const FSpawnTabArgs& Args);



	FReply OnGenerateLevel();
	void OnSizeChanged(const FText& InText, ETextCommit::Type CommitType);
	void OnMinesChanged(const FText& InText, ETextCommit::Type CommitType);

	void OnGameEnd(bool Win);


	static const FName MainTabId;
	static const FName SettingsTabId;
	static const FName GameTabId;

	TSharedPtr<SDockTab> MainTab;
	TSharedPtr<SWidget> MainTabContent;
	TSharedPtr<SDockTab> SettingsTab;
	TSharedPtr<SDockTab> GameTab;

	TSharedPtr<FUICommandList> MinesweeperCommands;
	TSharedPtr<FTabManager> MinesweeperTabManager;
	TSharedPtr<FWorkspaceItem> WorkspaceItem;
	TSharedPtr<FTabManager::FLayout> MinesweeperLayout;

	FMinesweeperLevel* Level;
	TSharedPtr<SEditableTextBox> RowsBox;
	TSharedPtr<SEditableTextBox> ColumnsBox;
	TSharedPtr<SEditableTextBox> MinesBox;
	TSharedPtr<SWrapBox> GameBox;

	TSharedPtr<SEditableTextBox> LogBox;





};
