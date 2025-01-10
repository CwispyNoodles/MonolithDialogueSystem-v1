﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraphNode_Response.h"

void UDialogueGraphNode_Response::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("PinCategory_Default"), TEXT("Response In"));
	CreatePin(EGPD_Output, TEXT("PinCategory_Default"), TEXT("Response Out"));
}
