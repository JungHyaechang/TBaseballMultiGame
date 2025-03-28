// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatSystemWidget.h"
#include "ChatEntryWidget.h"
#include "ChatComponent.h"
#include "Components/ScrollBox.h"
#include "BBPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


void UChatSystemWidget::NativeConstruct()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		ChatComponentRef = PC->FindComponentByClass<UChatComponent>();
	}
}

void UChatSystemWidget::Scroll(bool bUp)
{
	if (!ChatScrollBox) return;

	float Offset = ChatScrollBox->GetScrollOffset();
	float End = ChatScrollBox->GetScrollOffsetOfEnd();
	float ScrollStep = 60.f;

	Offset += (bUp ? -1.f : 1.f) * ScrollStep;
	Offset = FMath::Clamp(Offset, 0.f, End);
	ChatScrollBox->SetScrollOffset(Offset);
}

void UChatSystemWidget::AddChatMessage(const FString& Message)
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

bool UChatSystemWidget::IsChatHovered() const
{
	return IsHovered();
}

