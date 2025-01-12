﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "DialogueGraphNode_Response.generated.h"

/**
 * 
 */
UCLASS()
class MONOLITHDIALOGUESYSTEMEDITOR_API UDialogueGraphNode_Response : public UDialogueGraphNode
{
	GENERATED_BODY()

public: // UEdGraphNode Interface
	virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override { return FText::FromString("Response"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Green); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void AllocateDefaultPins() override;
};
