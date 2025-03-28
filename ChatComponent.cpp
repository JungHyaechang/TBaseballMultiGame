// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatComponent.h"
#include "ChatBoxWidget.h"
#include "ChatUIWidget.h"
#include "ChatSystemWidget.h"
#include "BBGameMode.h"
#include "BBGameState.h"
#include "BBPlayerState.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

UChatComponent::UChatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChatComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC || !PC->IsLocalPlayerController())
	{
		return;
	}

	if (ChatUIWidgetClass)
	{
		ChatUIRef = CreateWidget<UChatUIWidget>(PC, ChatUIWidgetClass);

		if (ChatUIRef)
		{
			ChatUIRef->AddToViewport();

			ChatBoxRef = ChatUIRef->GetChatBox();
			ChatSystemRef = ChatUIRef->GetChatSystem();
		}
	}

	if (PC)
	{
		PC->InputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &UChatComponent::EnterPressed);
		PC->InputComponent->BindKey(EKeys::MouseScrollUp, IE_Pressed, this, &UChatComponent::ScrollUp);
		PC->InputComponent->BindKey(EKeys::MouseScrollDown, IE_Pressed, this, &UChatComponent::ScrollDown);
	}
}

UChatUIWidget* UChatComponent::GetChatUI()
{

	if (!ChatUIRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatSystemWidget is not Assigned"))
			return nullptr;
	}

	return ChatUIRef;
}

void UChatComponent::EnterPressed()
{
	if (ChatBoxRef)
	{
		ChatBoxRef->FocusChat();
	}
}

void UChatComponent::ScrollUp()
{
	if (ChatBoxRef && ChatBoxRef->IsChatHovered())
	{
		ChatBoxRef->Scroll(true);
	}
	else if (ChatSystemRef && ChatSystemRef->IsChatHovered())
	{
		ChatSystemRef->Scroll(true);
	}
}

void UChatComponent::ScrollDown()
{
	if (ChatBoxRef && ChatBoxRef->IsChatHovered())
	{
		ChatBoxRef->Scroll(false);
	}
	else if (ChatSystemRef && ChatSystemRef->IsChatHovered())
	{
		ChatSystemRef->Scroll(false);
	}
}

void UChatComponent::SendChatMessageOnServer_Implementation(const FString& Message)
{
	UWorld* World = GetWorld();
	if (!World) return;

	AGameState* GameState = Cast<AGameState>(World->GetGameState());
	if (!GameState) return;

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (!PlayerState) continue;

		APlayerController* PC = Cast<APlayerController>(PlayerState->GetOwner());
		if (!PC) continue;

		UChatComponent* ChatComponent = PC->FindComponentByClass<UChatComponent>();
		if (ChatComponent)
		{
			ChatComponent->AddChatMessageOnOwningClient(Message);
		}
	}
}

void UChatComponent::AddChatMessageOnOwningClient_Implementation(const FString& Message)
{
	if (ChatBoxRef)
	{
		ChatBoxRef->AddChatMessage(Message);
	}
}

void UChatComponent::AddSystemChatMessageOnOwningClient_Implementation(const FString& Message)
{
	if (ChatSystemRef)
	{
		ChatSystemRef->AddChatMessage(Message);
	}
}

void UChatComponent::HandlePlayerGuess_Implementation(const FString& Input)
{
	if (ABBGameMode* BBGM = GetWorld()->GetAuthGameMode<ABBGameMode>())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
		BBGM->ProcessPlayerInput(PlayerController, Input);
	}
}


