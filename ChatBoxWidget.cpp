// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatBoxWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "ChatEntryWidget.h"
#include "ChatComponent.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "BBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/TextBlock.h"


void UChatBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputTextBox)
	{
		InputTextBox->OnTextCommitted.AddDynamic(this, &UChatBoxWidget::OnChatCommitted);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		ChatComponentRef = PC->FindComponentByClass<UChatComponent>();
	}
}

void UChatBoxWidget::AddChatMessage(const FString& Message)
{
	if (!ChatEntryClass && !ChatScrollBox) return;

	UChatEntryWidget* Entry = CreateWidget<UChatEntryWidget>(this, ChatEntryClass);
	if (Entry)
	{
		Entry->InMessage = Message;
		ChatScrollBox->AddChild(Entry);
		ChatScrollBox->ScrollToEnd();
	}
}

void UChatBoxWidget::FocusChat()
{
	if (!InputTextBox) return;

	bChatFocused = true;
	InputTextBox->SetIsEnabled(true);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	InputTextBox->SetUserFocus(PC);

	if (PC)
	{
		FInputModeGameAndUI InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void UChatBoxWidget::ExitChat()
{
	if (!InputTextBox) return;

	bChatFocused = false;
	InputTextBox->SetText(FText::GetEmpty());
	InputTextBox->SetIsEnabled(false);

	if (APlayerController* PC = GetOwningPlayer())
	{	
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void UChatBoxWidget::Scroll(bool bUp)
{
	if (!ChatScrollBox) return;

	float Offset = ChatScrollBox->GetScrollOffset();
	float End = ChatScrollBox->GetScrollOffsetOfEnd();
	float ScrollStep = 60.f;

	Offset += (bUp ? -1.f : 1.f) * ScrollStep;
	Offset = FMath::Clamp(Offset, 0.f, End);
	ChatScrollBox->SetScrollOffset(Offset);
}

bool UChatBoxWidget::IsChatHovered() const
{
	return IsHovered();
}


void UChatBoxWidget::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter || !ChatComponentRef) return;

	FString CleanText = Text.ToString().TrimStartAndEnd();
	if (CleanText.IsEmpty())
	{
		ExitChat();
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	APlayerState* PS = PC->GetPlayerState<APlayerState>();

	FString Name = PS ? PS->GetPlayerName() : TEXT("Unknown");
	FString Message = FString::Printf(TEXT("%s: %s"), *Name, *CleanText);

	ChatComponentRef->SendChatMessageOnServer(Message);
	ExitChat();

	ChatComponentRef->HandlePlayerGuess(CleanText);
}



