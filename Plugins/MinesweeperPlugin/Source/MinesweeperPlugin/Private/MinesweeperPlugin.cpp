// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPlugin.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

const FName FMinesweeperPluginModule::MainTabId(TEXT("MinesweeperMain"));
const FName FMinesweeperPluginModule::SettingsTabId(TEXT("MinesweeperSettings"));
const FName FMinesweeperPluginModule::GameTabId(TEXT("MinesweeperGame"));

void FMinesweeperPluginModule::StartupModule()
{
	FMinesweeperStyleSet::Initialize();


	FMinesweeperCommands::Register();
	MinesweeperCommands = MakeShared<FUICommandList>();
	MinesweeperCommands->MapAction(FMinesweeperCommands::Get().MinesweeperCommand,
		FExecuteAction::CreateRaw(this, &FMinesweeperPluginModule::OpenMinesweeperWindow));


	FLevelEditorModule& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	TSharedPtr<FExtender> MainToolbarExtender = MakeShared<FExtender>();

	MainToolbarExtender->AddToolBarExtension(TEXT("Content"),
		EExtensionHook::Before,
		MinesweeperCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FMinesweeperPluginModule::AddToolbarButton));

	LevelEditor.GetToolBarExtensibilityManager()->AddExtender(MainToolbarExtender);
	
	FGlobalTabmanager::Get()->RegisterTabSpawner(MainTabId, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginModule::SpawnMainTab))
		.SetDisplayName(FText::FromString(TEXT("Minesweeper")));

	Level = new FMinesweeperLevel(3, 3, 1, 20.f,20.f);
	Level->OnMinesweeperEnd.BindRaw(this, &FMinesweeperPluginModule::OnGameEnd);

}

void FMinesweeperPluginModule::ShutdownModule()
{
	MinesweeperTabManager->UnregisterTabSpawner(SettingsTabId);
	MinesweeperTabManager->UnregisterTabSpawner(GameTabId);
	MinesweeperTabManager.Reset();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MainTabId);

	FMinesweeperStyleSet::Shutdown();

}


void FMinesweeperPluginModule::OpenMinesweeperWindow()
{

	FGlobalTabmanager::Get()->TryInvokeTab(MainTabId);
	Level->ResetLevel(GameBox.ToSharedRef(), Level->GetLevelSize(), true);

}


TSharedRef<SDockTab> FMinesweeperPluginModule::SpawnMainTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == MainTabId);

	MainTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Icon(FMinesweeperStyleSet::Get().GetBrush(TEXT("Minesweeper.Icon16")))
		.Label(LOCTEXT("MinesweeperMain", "Minesweeper"));


		MinesweeperTabManager = FGlobalTabmanager::Get()->NewTabManager(MainTab.ToSharedRef());

	

	
		WorkspaceItem = MinesweeperTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("MinesweeperWorkspace", "Minesweeper"));

		MinesweeperTabManager->RegisterTabSpawner(SettingsTabId, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginModule::SpawnSettingsTab))
			.SetDisplayName(FText::FromString(TEXT("Settings")))
			.SetGroup(WorkspaceItem.ToSharedRef());


		MinesweeperTabManager->RegisterTabSpawner(GameTabId, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginModule::SpawnGameTab))
			.SetDisplayName(FText::FromString(TEXT("Game")))
			.SetGroup(WorkspaceItem.ToSharedRef());



		MinesweeperLayout = MinesweeperTabManager->NewLayout(TEXT("MinesweeperLayout_Basev1"))
			->AddArea
			(

				MinesweeperTabManager->NewPrimaryArea()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->AddTab(SettingsTabId, ETabState::OpenedTab)

				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->AddTab(GameTabId, ETabState::OpenedTab)
				)

			);



	MainTabContent = MinesweeperTabManager->RestoreFrom(MinesweeperLayout.ToSharedRef(), MakeShared<SWindow>());
	MainTab->SetContent(MainTabContent.ToSharedRef());


	return MainTab.ToSharedRef();

}



TSharedRef<SDockTab> FMinesweeperPluginModule::SpawnSettingsTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == SettingsTabId);

	return SAssignNew(SettingsTab, SDockTab)
		.Icon(FMinesweeperStyleSet::Get().GetBrush(TEXT("Minesweeper.SettingsIcon")))
		.Label(LOCTEXT("MinesweeperSettings", "Settings"))
		.TabRole(ETabRole::PanelTab)
		[

			SNew(SBorder)
			.ColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.BorderColor")).GetSpecifiedColor())
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				[
					SNew(SSimpleGradient)
					.StartColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor1")).GetSpecifiedColor())
					.EndColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor1")).GetSpecifiedColor())

				]
				+SOverlay::Slot()
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMinesweeperStyleSet::Get().GetMargin(TEXT("Minesweeper.TabMargin")))
					[

						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[

							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Rows:")))
							.ColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HighlightColor")))

						]
						+SHorizontalBox::Slot()
						[

							SAssignNew(RowsBox, SEditableTextBox)
							.Text(FText::FromString(TEXT("3")))
							.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperPluginModule::OnSizeChanged))
							.BackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.EditColor")))
							.ForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.TextColor")))

						]
						+SHorizontalBox::Slot()
						.MaxWidth(5.f)
						.Padding(15.f,0.f,0.f,0.f)
						[

							SNew(SSeparator)
							.ColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")))

						]
						+SHorizontalBox::Slot()
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[

							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Columns:")))
							.ColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HighlightColor")))

						]
						+ SHorizontalBox::Slot()
						[

							SAssignNew(ColumnsBox, SEditableTextBox)
							.Text(FText::FromString(TEXT("3")))
							.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperPluginModule::OnSizeChanged))
							.BackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.EditColor")))
							.ForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.TextColor")))
						]
					
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMinesweeperStyleSet::Get().GetMargin(TEXT("Minesweeper.TabMargin")))
					[
						SNew(SBox)
						.Padding(FMinesweeperStyleSet::Get().GetMargin(TEXT("Minesweeper.TabMargin")))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(EHorizontalAlignment::HAlign_Center)
							[

								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Mines:")))
								.ColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HighlightColor")))

							]
							+ SHorizontalBox::Slot()
							[
							
								SAssignNew(MinesBox, SEditableTextBox)
								.Text(FText::FromString(TEXT("1")))
								.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperPluginModule::OnMinesChanged))
								.BackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.EditColor")))
								.ForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.TextColor")))

						

							]
						]

					]
					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMinesweeperStyleSet::Get().GetMargin(TEXT("Minesweeper.TabMargin")))
						[

							SNew(SButton)
							.Text(FText::FromString(TEXT("Generate Level")))
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.OnClicked(FOnClicked::CreateRaw(this, &FMinesweeperPluginModule::OnGenerateLevel))
							.ButtonColorAndOpacity(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")))
							.ForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HighlightColor")))
						

						]
					+ SVerticalBox::Slot()
						.VAlign(EVerticalAlignment::VAlign_Bottom)
						.Padding(FMinesweeperStyleSet::Get().GetMargin(TEXT("Minesweeper.TabMargin")))
						[
							SNew(SBorder)
							.BorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")))
							[
								SAssignNew(LogBox, SEditableTextBox)
								.Text(FText::FromString(Level->InstructionsMessage))
								.BackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")))
								.ReadOnlyForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HighlightColor")))
								.IsReadOnly(true)

							]


						]
				]
			]
		];


	

}


TSharedRef<SDockTab> FMinesweeperPluginModule::SpawnGameTab(const FSpawnTabArgs& Args)
{
	
	check(Args.GetTabId() == GameTabId);


	return SAssignNew(GameTab, SDockTab)
		.Icon(FMinesweeperStyleSet::Get().GetBrush(TEXT("Minesweeper.GameIcon")))
		.Label(LOCTEXT("MinesweeperGame", "Game"))
		.TabRole(ETabRole::PanelTab)
		[

			SNew(SBorder)
			.BorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.BorderColor")))
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				[
					SNew(SSimpleGradient)
					.StartColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")).GetSpecifiedColor())
					.EndColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.MainColor2")).GetSpecifiedColor())

				]
				+SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.BorderColor")))
					[

						SAssignNew(GameBox, SWrapBox)
						.Orientation(EOrientation::Orient_Horizontal)
						.PreferredSize(Level->CellWidth* Level->GetLevelRows())


					]


				]

			]


		];


		
	
}

void FMinesweeperPluginModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	FSlateIcon Icon = FSlateIcon(FMinesweeperStyleSet::Get().GetStyleSetName(), TEXT("Minesweeper.Icon48"));

	Builder.AddToolBarButton(FMinesweeperCommands::Get().MinesweeperCommand, NAME_None, 
		FMinesweeperCommands::Get().MinesweeperCommand->GetLabel(), 
		FMinesweeperCommands::Get().MinesweeperCommand->GetDescription(), Icon);


	

}


FReply FMinesweeperPluginModule::OnGenerateLevel()
{
	int32 Size = Level->GetLevelSize();

	if (Size != Level->LevelSlots.Num())
	{
		bool UpSize = Size > Level->LevelSlots.Num();

		Level->ResetLevel(GameBox.ToSharedRef(), Size, UpSize);

	}
	else
	{
		Level->ResetButtons();
		Level->ResetMinesIndexes();
		Level->AssignMinesIndexes();
		Level->AvailableCellsCounter = Level->GetLevelSize();
	}

	LogBox->SetText(FText::FromString(Level->InstructionsMessage));

	return FReply::Handled();

}


void FMinesweeperPluginModule::OnSizeChanged(const FText& InText, ETextCommit::Type CommitType)
{

	int32 Rows = FCString::Atoi(*RowsBox->GetText().ToString());
	int32 Cols = FCString::Atoi(*ColumnsBox->GetText().ToString());

	if (Rows <= 0 || Rows > 38)
	{
		return;
	}


	if (Cols <= 0 || Cols > 44)
	{
		return;
	}


	Level->ChangeSize(Rows, Cols);

}



void FMinesweeperPluginModule::OnMinesChanged(const FText& InText, ETextCommit::Type CommitType)
{

	int32 Mines = FCString::Atoi(*MinesBox->GetText().ToString());

	if (Mines <= 0 ||Mines >= Level->GetLevelSize())
	{
		return;
	}


	Level->ChangeMines(Mines);

}

void FMinesweeperPluginModule::OnGameEnd(bool Win)
{
	
	FString Message;
	
	if (Win)
	{
		Message = Level->InstructionsMessage + Level->WinMessage;

	}
	else 
	{

		Message = Level->InstructionsMessage + Level->LoseMessage;
	}

	LogBox->SetText(FText::FromString(Message));

}




#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperPluginModule, MinesweeperPlugin)