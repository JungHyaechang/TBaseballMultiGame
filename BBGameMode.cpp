// Fill out your copyright notice in the Description page of Project Settings.


#include "BBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "ChatComponent.h"
#include "BaseballUtility.h"
#include "BBPlayerState.h"
#include "BBGameState.h"
#include "BBJudgeComponent.h"
#include "BBPlayerController.h"

ABBGameMode::ABBGameMode()
{
	PlayerIndex = 1;
	CurrentTurnIndex = -1;
	CurrentTurnPlayer = nullptr;
	TurnDuration = 15;
	bGameStarted = false;
}

void ABBGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ABBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (APlayerState* PS = NewPlayer->PlayerState)
	{
		FString Name = FString::Printf(TEXT("Player%d"), PlayerIndex++);
		PS->SetPlayerName(Name);
	}
}

void ABBGameMode::ProcessPlayerInput(APlayerController* Sender, const FString& Input)
{
	if (!Input.StartsWith("/")) return;

	if (Input.Equals("/start", ESearchCase::IgnoreCase))
	{
		if (!bGameStarted)
		{
			bGameStarted = true;
			ResetGame();
		}
		else
		{
			SendMessageTargetPlayer(Sender, TEXT("Game has already started."));
		}
		return;
	}

	if (!bGameStarted)
	{
		SendMessageTargetPlayer(Sender, TEXT("Game has not started yet. Type '/start' to begin."));
		return;
	}

	if (Sender != CurrentTurnPlayer)
	{
		FString NotYourTurnMessage = TEXT("It's not your turn");
		SendMessageTargetPlayer(Sender, NotYourTurnMessage);
		return;
	}

	FString Guess = Input.RightChop(1);
	if (Guess.Len() != 3 || Guess.Contains(TEXT("0")) || TSet<TCHAR>(Guess).Num() != 3)
	{
		if (UChatComponent* Chat = Sender->FindComponentByClass<UChatComponent>())
		{
			Guess = "000";
		}
	}

	JudgeAndBroadcast(Sender, Guess);

	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	SwitchToNextTurn();
}

void ABBGameMode::ResetGame()
{
	SecretNumber = BaseballUtility::GenerateRandomNumber();
	UE_LOG(LogTemp, Warning, TEXT("SecretNumber: %s"), *SecretNumber);

	TurnPlayers.Empty();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (ABBPlayerState* BBPS = Cast<ABBPlayerState>(PlayerState))
		{
			BBPS->TryCount = 0;
			UE_LOG(LogTemp, Warning, TEXT("BBPlayerState Trycount Reset : %d"), BBPS->TryCount);
		}

		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerState->GetOwner()))
		{
			TurnPlayers.Add(PlayerController);
		}
	}

	NotifyGameStart();

	if (TurnPlayers.Num() > 0)
	{
		CurrentTurnIndex = 0;
		StartTurn(TurnPlayers[CurrentTurnIndex]);
	}
}

void ABBGameMode::StartTurn(APlayerController* NewTurnPlayer)
{
	CurrentTurnPlayer = NewTurnPlayer;
	FString PlayerName = NewTurnPlayer->PlayerState->GetPlayerName();
	BroadcastMessage(FString::Printf(TEXT("It's %s's Turn!! (%d Seconds)"), *PlayerName, TurnDuration));
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(TurnUpdateHandle);

	ABBGameState* BBGS = GetGameState<ABBGameState>();
	if (BBGS)
	{
		BBGS->RemainingTime = TurnDuration;
	}

	GetWorldTimerManager().SetTimer(
		TurnTimerHandle,
		this,
		&ABBGameMode::OnTurnTimeout,
		TurnDuration,
		false
	);
	GetWorldTimerManager().SetTimer(
		TurnUpdateHandle,
		this,
		&ABBGameMode::UpdateRemainingTime,
		1.0f,
		true
	);
}

void ABBGameMode::OnTurnTimeout()
{
	if (!CurrentTurnPlayer) return;

	ABBPlayerState* BBPS = Cast<ABBPlayerState>(CurrentTurnPlayer->PlayerState);
	if (BBPS && BBPS->GetTryCount() < 3)
	{
		BBPS->AddTry();
		FString PlayerName = BBPS->GetPlayerName();
		BroadcastMessage(FString::Printf(TEXT("%s ran out of time! (Try %d/3)"), *PlayerName, BBPS->GetTryCount()));
		FString EndLine = TEXT("-------------------------------");
		BroadcastMessage(EndLine);
		CheckWinOrDraw(CurrentTurnPlayer, 0);
	}

	SwitchToNextTurn();
}

void ABBGameMode::SwitchToNextTurn()
{
	if (!bGameStarted || TurnPlayers.Num() == 0) return;
	CurrentTurnIndex = (CurrentTurnIndex + 1) % TurnPlayers.Num();
	StartTurn(TurnPlayers[CurrentTurnIndex]);
}

void ABBGameMode::JudgeAndBroadcast(APlayerController* Sender, const FString& Guess)
{
	UE_LOG(LogTemp, Warning, TEXT("JudgeAndBroadcast Function Called"));

	APlayerState* PlayerState = Sender->PlayerState;
	FString PlayerName = PlayerState->GetPlayerName();
	if (ABBPlayerState* BBPS = Cast<ABBPlayerState>(PlayerState))
	{
		if (BBPS->GetTryCount() >= 3)
		{
			FString NotifyTryCountOver = FString::Printf(TEXT("%s has already used 3 tries!"), *PlayerName);
			SendMessageTargetPlayer(Sender, NotifyTryCountOver);
			return; 
		}

		BBPS->AddTry();

		int32 Strike = 0, Ball = 0;
		FString Result = BaseballUtility::JudgeGuess(SecretNumber, Guess, Strike, Ball);
		FString ResultMessage = PlayerName + TEXT(": ") + Result + FString::Printf(TEXT(" (Try %d/3)"), BBPS->GetTryCount());

		UE_LOG(LogTemp, Warning, TEXT("Result: %d Strike, %d Ball"), Strike, Ball);
		UE_LOG(LogTemp, Warning, TEXT("Try Count now: %d"), BBPS->GetTryCount());

		BroadcastMessage(ResultMessage);
		FString EndLine = TEXT("-------------------------------");
		BroadcastMessage(EndLine);
		CheckWinOrDraw(Sender, Strike);
	}
}

void ABBGameMode::CheckWinOrDraw(APlayerController* Sender, int32 Strikes)
{
	if (!Sender || !Sender->PlayerState) return;

	ABBPlayerState* SenderPS = Cast<ABBPlayerState>(Sender->PlayerState);
	if (!SenderPS) return;

	if (Strikes == 3)
	{
		FString Winner = SenderPS->GetPlayerName();
		BroadcastMessage(FString::Printf(TEXT("Winner is %s. Congretulation!"), *Winner));
		BroadcastMessage(TEXT("Type /start to Play Again."));
		GetWorldTimerManager().ClearTimer(TurnUpdateHandle);

		SenderPS->AddWin();

		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (ABBPlayerState* BBPS = Cast<ABBPlayerState>(PS))
			{
				if (ABBPlayerController* BBPC = Cast<ABBPlayerController>(BBPS->GetOwner()))
				{
					BBPC->Client_UpdateAllWinCounts();
				}
			}
		}

		bGameStarted = false;
		return;
	}

	// Check All Players TryCount 3
	bool bAllTried = true;
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ABBPlayerState* BBPS = Cast<ABBPlayerState>(PS))
		{
			if (BBPS->GetTryCount() < 3)
			{
				bAllTried = false;
				break;
			}
		}
	}

	if (bAllTried)
	{
		BroadcastMessage(TEXT("Draw!! Nobody got 3 Strike."));
		BroadcastMessage(TEXT("Type /start to Play Again."));
		bGameStarted = false;
		GetWorldTimerManager().ClearTimer(TurnUpdateHandle);
	}
}

void ABBGameMode::BroadcastMessage(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("BraodcastMessage Function is Called"));
	UE_LOG(LogTemp, Warning, TEXT("Broadcasting: %s"), *Message);
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (APlayerController* PC = Cast<APlayerController>(PlayerState->GetOwner()))
		{
			if (UChatComponent* Chat = PC->FindComponentByClass<UChatComponent>())
			{
				Chat->AddSystemChatMessageOnOwningClient(Message);
			}
		}
	}
}

void ABBGameMode::SendMessageTargetPlayer(APlayerController* Sender, const FString& Message)
{
	if (Sender)
	{
		if (UChatComponent* Chat = Sender->FindComponentByClass<UChatComponent>())
		{
			Chat->AddSystemChatMessageOnOwningClient(Message);
		}
	}
}

void ABBGameMode::NotifyGameStart()
{
	FString NotifyGameStartedStartLine = TEXT("======= NewGame Start!! =======");
	BroadcastMessage(NotifyGameStartedStartLine);
	FString NotifyGameMethod = TEXT("Type '/Number' and Guess!");
	BroadcastMessage(NotifyGameMethod);
	FString NotifyGameStartedEndLine = TEXT("===============================");
	BroadcastMessage(NotifyGameStartedEndLine);
}

void ABBGameMode::UpdateRemainingTime()
{
	ABBGameState* BBGS = GetGameState<ABBGameState>();
	if (BBGS)
	{
		BBGS->RemainingTime--;
		if (BBGS->RemainingTime <= 0)
		{
			GetWorldTimerManager().ClearTimer(TurnUpdateHandle);
		}
	}
}
