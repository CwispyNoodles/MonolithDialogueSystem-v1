﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraphSchema.h"

#include "DialogueGraph.h"
#include "DialogueGraphNode_Alias.h"
#include "DialogueGraphNode_Query.h"
#include "DialogueGraphNode_Response.h"
#include "DialogueGraphNode_Root.h"
#include "DialogueNodeData_Alias.h"
#include "DialogueNodeData_Dialogue.h"

UEdGraphNode* FDialogueGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(ParentGraph);
	
	UEdGraphNode* MyNode = CreateNode(ParentGraph, Location);
	UDialogueGraphNode_Base* DialogueNode = Cast<UDialogueGraphNode_Base>(MyNode);
	// MyNode->SetDialogueNodeData(NewObject<UDialogueNodeData_Base>(MyNode));

	if (FromPin && DialogueNode->GetDefaultInputPin())
	{
		MyNode->GetSchema()->TryCreateConnection(FromPin, DialogueNode->GetDefaultInputPin());
	}

	DialogueGraph->Modify();
	DialogueGraph->AddNode(MyNode, true, true);

	return MyNode;
}

UEdGraphNode* FDialogueGraphSchemaAction_NewNode::CreateNode(UEdGraph* InParentGraph, FVector2D InLocation)
{
	UDialogueGraphNode_Base* MyNode = NewObject<UDialogueGraphNode_Base>(InParentGraph, ClassTemplate);
	MyNode->CreateNewGuid();
	MyNode->SetPosition(InLocation);
	MyNode->InitializeNodeData();

	MyNode->AllocateDefaultPins();
	return MyNode;
}

UEdGraphNode* FDialogueGraphSchemaAction_NewNode_AliasOut::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                                         const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* MyNode = CreateNode(ParentGraph, Location);
	UDialogueGraphNode_Alias_Out* AliasNode = Cast<UDialogueGraphNode_Alias_Out>(MyNode);
	UDialogueNodeData_Alias_Out* NodeData = Cast<UDialogueNodeData_Alias_Out>(AliasNode->GetDialogueNodeData());
	NodeData->AliasName = AliasName;

	ParentGraph->Modify();
	ParentGraph->AddNode(MyNode, true, true);

	return MyNode;
}

UEdGraphNode* FDialogueGraphSchemaAction_NewNode_Response::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* MyNode = FDialogueGraphSchemaAction_NewNode::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
	UDialogueGraphNode_Response* ResponseNode = Cast<UDialogueGraphNode_Response>(MyNode);

	ResponseNode->GetResponseNodeData()->ResponseTexts.Add(FText());
	ResponseNode->SyncPinsWithResponses();

	return ResponseNode;
	
}

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UDialogueGraphNode_Root> NodeCreator(Graph);
	UDialogueGraphNode_Root* MyNode = NodeCreator.CreateNode(true, UDialogueGraphNode_Root::StaticClass());
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
	}

	if (A->Direction == B->Direction) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
	}

	// Root Node Conditions
	if (A->GetOwningNode()->IsA(UDialogueGraphNode_Root::StaticClass()))
	{
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Alias_In::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
		}
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Query::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Query Conditions
	if (A->GetOwningNode()->IsA(UDialogueGraphNode_Query::StaticClass()))
	{
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Response::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Alias Out Conditions
	if (A->GetOwningNode()->IsA(UDialogueGraphNode_Alias_Out::StaticClass()))
	{
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Query::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Response conditions
	if (A->GetOwningNode()->IsA(UDialogueGraphNode_Response::StaticClass()))
	{
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Query::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
		}
		if (B->GetOwningNode()->IsA(UDialogueGraphNode_Alias_In::StaticClass()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
}

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FDialogueGraphSchemaAction_NewNode> NewQueryNodeAction(
		new FDialogueGraphSchemaAction_NewNode(
			UDialogueGraphNode_Query::StaticClass(),
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Query Node")),
			FText::FromString(TEXT("Creates new Query Node")),
			FText::GetEmpty(),
			0));

	TSharedPtr<FDialogueGraphSchemaAction_NewNode_Response> NewResponseNodeAction(
		new FDialogueGraphSchemaAction_NewNode_Response(
			UDialogueGraphNode_Response::StaticClass(),
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Response Node")),
			FText::FromString(TEXT("Creates new Response Node")),
			FText::GetEmpty(),
			0));

	TSharedPtr<FDialogueGraphSchemaAction_NewNode> NewAliasInNodeAction(
		new FDialogueGraphSchemaAction_NewNode(
			UDialogueGraphNode_Alias_In::StaticClass(),
			FText::FromString(TEXT("Nodes|Alias")),
			FText::FromString(TEXT("New Alias Input")),
			FText::FromString(TEXT("Creates new Alias Input")),
			FText::GetEmpty(),
			0));

	ContextMenuBuilder.AddAction(NewQueryNodeAction);
	ContextMenuBuilder.AddAction(NewResponseNodeAction);
	ContextMenuBuilder.AddAction(NewAliasInNodeAction);

	if (const UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(ContextMenuBuilder.CurrentGraph))
	{
		TArray<FString> Subjects;
		DialogueGraph->DialogueGraphData->AliasCounter.GetSubjects(Subjects);
		for (FString Alias : Subjects)
		{
			// Only want one alias output per alias at a time.
			if (DialogueGraph->DialogueGraphData->AliasToOutputMap.Find(Alias))
			{
				continue;
			}
			FString InMenuDesc = FString(TEXT("New '{0}' Alias Output"));
			InMenuDesc = FString::Format(*InMenuDesc, {Alias});
			
			TSharedPtr<FDialogueGraphSchemaAction_NewNode_AliasOut> NewAliasOutNodeAction(
				new FDialogueGraphSchemaAction_NewNode_AliasOut(
					UDialogueGraphNode_Alias_Out::StaticClass(),
					FText::FromString(Alias),
					FText::FromString(TEXT("Nodes|Alias")),
					FText::FromString(InMenuDesc),
					FText::FromString(TEXT("Creates new Alias Output")),
					FText::GetEmpty(),
					0));

			ContextMenuBuilder.AddAction(NewAliasOutNodeAction);
		}
	}
}
