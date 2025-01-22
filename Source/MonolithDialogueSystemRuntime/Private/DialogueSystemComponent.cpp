﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemComponent.h"

#include "DialogueGraphData.h"
#include "DialogueInstance.h"
#include "DialogueNodeData_Dialogue.h"


// Sets default values for this component's properties
UDialogueSystemComponent::UDialogueSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

FDialogueHandle UDialogueSystemComponent::StartDialogue()
{
	if (!Dialogue)
	{
		return FDialogueHandle();
	}

	UDialogueRuntimeGraph* Graph = Dialogue->Graph;
	UDialogueRuntimeNode* RootNode = Graph->RootNode;

	FDialogueHandle DialogueHandle;

	for (UDialogueRuntimePin* Pin : RootNode->Pins)
	{
		UDialogueRuntimePin* Connection = Pin->Connection;
		if (!Connection)
			continue;

		UDialogueRuntimeNode* ConnectionParent = Connection->Parent;
		if (!ConnectionParent)
			continue;

		if (UDialogueNodeData_Query* QueryData = Cast<UDialogueNodeData_Query>(ConnectionParent->NodeData))
		{
			DialogueHandle.QueryText = QueryData->QueryText;
		}
	}

	// if (UDialogueRuntimeNode* Query = RootNode->Pins[0]->Parent)
	// {
	// 	// Assume for now that the next node will be a query node
	// 	UDialogueNodeData_Query* QueryData = Cast<UDialogueNodeData_Query>(Query->NodeData);
	// 	DialogueHandle.QueryText = QueryData->QueryText;
	//
	// 	if (UDialogueRuntimeNode* Responses = Query->Pins[1]->Parent)
	// 	{
	// 		UDialogueNodeData_Response* ResponseData = Cast<UDialogueNodeData_Response>(Responses->NodeData);
	// 		DialogueHandle.ResponseTexts = ResponseData->ResponseTexts;
	// 	}
	// 	
	// 	// if (Parent->NodeType == EDialogueNodeType::AliasIn)
	// 	// {
	// 	// 	WorkingDialogue->Graph->GraphData->AliasToOutputMap.Find(Parent->NodeData);
	// 	// }
	// }

	return DialogueHandle;
}


// Called when the game starts
void UDialogueSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDialogueSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

