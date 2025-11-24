// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatInput.h"

#include "Components/EditableTextBox.h"

#include "Player/CBPlayerController.h"

void UChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputCommitted);
	}
}

void UChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputCommitted);
	}
}

void UChatInput::OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (OwningPlayerController != nullptr)
		{
			ACBPlayerController* OwningCBPlayerController = Cast<ACBPlayerController>(OwningPlayerController);
			if (OwningCBPlayerController != nullptr)
			{
				OwningCBPlayerController->CommitInChatMsg(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
