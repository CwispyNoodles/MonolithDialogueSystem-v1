﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraphTabSummoner.h"

#include "DialogueInstanceEditor.h"

#define LOCTEXT_NAMESPACE "DialogueGraphTabSummoner"

const FName DialogueGraphTabId = FName(TEXT("DialogueGraphTabId"));

FDialogueGraphTabSummoner::FDialogueGraphTabSummoner(TSharedPtr<FDialogueInstanceEditor> InDialogueInstanceEditor)
	: FWorkflowTabFactory(DialogueGraphTabId, InDialogueInstanceEditor)
{
	DialogueInstanceEditor = InDialogueInstanceEditor;
	TabLabel = LOCTEXT("DialogueGraphTabLabel", "Dialogue Graph");
	ViewMenuDescription = LOCTEXT("DialogueGraph_ViewMenu_Desc", "Dialogue Graph");
	ViewMenuTooltip = LOCTEXT("DialogueGraph_ViewMenu_ToolTip", "Show the Dialogue Graph");
}

TSharedRef<SWidget> FDialogueGraphTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueInstanceEditor> Editor = DialogueInstanceEditor.Pin();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged.BindRaw(Editor.Get(), &FDialogueInstanceEditor::OnGraphSelectionChanged);
	
	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEvents)
			.GraphToEdit(Editor->GetWorkingGraph());

	Editor->SetWorkingGraphEditor(GraphEditor);

	return GraphEditor.ToSharedRef();
}

FText FDialogueGraphTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("GetTabToolTipText Test"));
}

#undef LOCTEXT_NAMESPACE