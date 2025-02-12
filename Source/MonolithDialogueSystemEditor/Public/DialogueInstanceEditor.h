﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphPanelNodeFactory.h"
#include "DialogueGraphPanelPinFactory.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UDialogueGraphNode_Base;
class UDialogueInstance;
class IDetailsView;
/**
 * 
 */
class MONOLITHDIALOGUESYSTEMEDITOR_API FDialogueInstanceEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

public:
	void InitDialogueInstanceEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);
	void SetSelectedNodeDetailsView(TSharedPtr<IDetailsView> DetailsView);
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);
	void OnWorkingAssetPreSave();

	void SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InWorkingGraphEditor) { WorkingGraphEditor = InWorkingGraphEditor; }

	UDialogueInstance* GetWorkingAsset() { return WorkingAsset; }
	UEdGraph* GetWorkingGraph() { return WorkingGraph; }

private:
	void BindGraphCommands();
	void DeleteSelectedNodes();
	bool CanDeleteNodes(); 
	void OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event);
	
	UDialogueInstance* WorkingAsset = nullptr;
	UEdGraph* WorkingGraph = nullptr;
	TSharedPtr<FDialogueGraphPanelNodeFactory> DialogueGraphPanelNodeFactory;
	TSharedPtr<FDialogueGraphPanelPinFactory> DialogueGraphPanelPinFactory;
	TSharedPtr<IDetailsView> SelectedNodeDetailsView;
	TSharedPtr<SGraphEditor> WorkingGraphEditor = nullptr;

protected:
	UDialogueGraphNode_Base* GetSelectedNode(const FGraphPanelSelectionSet& Selection);
	void SaveGraph();
	void LoadGraph();
	
public: // FAssetEditorToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override;
	virtual void OnClose() override;
};

