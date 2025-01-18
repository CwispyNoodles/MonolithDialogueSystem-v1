﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraph.h"

#include "DialogueGraphNode_Alias.h"
#include "DialogueNodeData_Alias.h"
#include "GraphEditAction.h"

UDialogueGraph::UDialogueGraph()
{
	AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateUObject(this, &UDialogueGraph::HandleThisGraphModified));
}

void UDialogueGraph::HandleThisGraphModified(const FEdGraphEditAction& InEditAction)
{
	if (InEditAction.Action == GRAPHACTION_RemoveNode)
	{
		for (const UEdGraphNode* RemovedNode : InEditAction.Nodes)
		{
			if (const UDialogueGraphNode_Alias_In* AliasNode = Cast<UDialogueGraphNode_Alias_In>(RemovedNode))
			{
				UDialogueNodeData_Alias* NodeData = Cast<UDialogueNodeData_Alias>(AliasNode->GetDialogueNodeData());
				DialogueGraphData->AliasCounter.Unsubscribe(NodeData->Ticket);
			}
		}
	}

	if (InEditAction.Action == GRAPHACTION_EditNode)
	{
		for (const UEdGraphNode* RemovedNode : InEditAction.Nodes)
		{
			if (const UDialogueGraphNode_Alias_In* AliasNode = Cast<UDialogueGraphNode_Alias_In>(RemovedNode))
			{
				UDialogueNodeData_Alias* NodeData = Cast<UDialogueNodeData_Alias>(AliasNode->GetDialogueNodeData());
				DialogueGraphData->AliasCounter.Unsubscribe(NodeData->Ticket);
				NodeData->Ticket = DialogueGraphData->AliasCounter.Subscribe(NodeData->GetAliasName().ToString());
			}
		}
	}
	
}

void UDialogueGraph::AddNode(UEdGraphNode* NodeToAdd, bool bUserAction, bool bSelectNewNode)
{
	Super::AddNode(NodeToAdd, bUserAction, bSelectNewNode);
	
	if (UDialogueGraphNode_Alias_In* AliasNode = Cast<UDialogueGraphNode_Alias_In>(NodeToAdd))
	{
		UDialogueNodeData_Alias* NodeData = Cast<UDialogueNodeData_Alias>(AliasNode->GetDialogueNodeData());
		NodeData->Ticket = DialogueGraphData->AliasCounter.Subscribe(NodeData->GetAliasName().ToString());
	}

	
}

