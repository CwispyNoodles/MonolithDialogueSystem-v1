﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraphNode.h"

void UDialogueGraphNode::SetPosition(FVector2D InPos)
{
	NodePosX = InPos.X;
	NodePosY = InPos.Y;
}

void UDialogueGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Dialog Node Actions")));
	UDialogueGraphNode* node = (UDialogueGraphNode*)this;
	Section.AddMenuEntry(
		TEXT("DeleteEntry"),
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Deletes the node")),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda(
			[node] () {
				node->GetGraph()->RemoveNode(node);
			}
		))
	);
}
