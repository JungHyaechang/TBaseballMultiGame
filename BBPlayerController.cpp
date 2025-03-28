// Fill out your copyright notice in the Description page of Project Settings.


#include "BBPlayerController.h"
#include "BBGameMode.h"
#include "ChatComponent.h"
#include "ChatUIWidget.h"
#include "Kismet/GameplayStatics.h"


void ABBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BBPlayerController BeginPlay Fuction Excuted"));
}

void ABBPlayerController::ApplyWinscountToChatUIAfterDelay()
{
	if (UChatComponent* ChatComp = FindComponentByClass<UChatComponent>())
	{
		if (UChatUIWidget* ChatUI = ChatComp->GetChatUI())
		{
			ChatUI->UpdateAllWinText();
		}
	}
}

void ABBPlayerController::Client_UpdateAllWinCounts_Implementation()
{
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &ABBPlayerController::ApplyWinscountToChatUIAfterDelay, 2.0f, false);
}

