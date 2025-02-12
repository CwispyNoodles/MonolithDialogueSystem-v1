// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonolithDialogueSystemEditor.h"

#include "AssetToolsModule.h"
#include "DialogueInstanceActions.h"

#define LOCTEXT_NAMESPACE "FMonolithDialogueSystemEditorModule"

void FMonolithDialogueSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogueInstance")), LOCTEXT("DialogueInstanceCategory", "Dialogue"));
	TSharedPtr<FDialogueInstanceActions> DialogueInstanceActions = MakeShareable(new FDialogueInstanceActions(AssetType));

	AssetTools.RegisterAssetTypeActions(DialogueInstanceActions.ToSharedRef());
	
}

void FMonolithDialogueSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMonolithDialogueSystemEditorModule, MonolithDialogueSystemEditor)