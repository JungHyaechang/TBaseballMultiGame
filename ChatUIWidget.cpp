// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatUIWidget.h"
#include "ChatBoxWidget.h"
#include "BBGameState.h"
#include "BBPlayerState.h"
#include "Components/TextBlock.h"

UChatBoxWidget* UChatUIWidget::GetChatBox()
{
	if (!ChatBoxWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatBoxWidget is not Assigned"))
		return nullptr;
	}

	return ChatBoxWidget;
}

UChatSystemWidget* UChatUIWidget::GetChatSystem()
{
	if (!ChatSystemWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatSystemWidget is not Assigned"))
		return nullptr;
	}

	return ChatSystemWidget;
}

UTextBlock* UChatUIWidget::GetTurnTimerText()
{
	return TurnTimerText;
}

void UChatUIWidget::UpdateAllWinText()
{
	if (ABBGameState* GS = GetWorld()->GetGameState<ABBGameState>())
	{
		TArray<ABBPlayerState*> SortedPlayers;

		for (APlayerState* PS : GS->PlayerArray)
		{
			if (ABBPlayerState* BBPS = Cast<ABBPlayerState>(PS))
			{
				SortedPlayers.Add(BBPS);
			}
		}

		SortedPlayers.Sort([](const ABBPlayerState& A, const ABBPlayerState& B) {
			return A.GetPlayerName() < B.GetPlayerName();
			});

		FString WinInfo;
		for (ABBPlayerState* BBPS : SortedPlayers)
		{
			WinInfo += FString::Printf(TEXT("%s: %d Wins\n"),
				*BBPS->GetPlayerName(), BBPS->WinCount);
		}

		if (AllPlayerWinsText)
		{
			AllPlayerWinsText->SetText(FText::FromString(WinInfo));
		}
	}
}


void UChatUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ABBGameState* BBGS = GetWorld()->GetGameState<ABBGameState>())
	{
		if (TurnTimerText)
		{
			TurnTimerText->SetText(FText::Format(
				FText::FromString(TEXT("Remaining Time : {0}")),
				FText::AsNumber(BBGS->GetRemainingTime())
			));
		}
	}
}
