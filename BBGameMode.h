// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BBGameMode.generated.h"


UCLASS()
class MULTIPLAYNETWORK_API ABBGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	ABBGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void ProcessPlayerInput(APlayerController* Sender, const FString& Input);
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int32 TurnDuration;

private:
	FString SecretNumber;
	int32 PlayerIndex;
	FTimerHandle TurnTimerHandle;
	FTimerHandle TurnUpdateHandle;
	TArray<APlayerController*> TurnPlayers;
	int32 CurrentTurnIndex;
	APlayerController* CurrentTurnPlayer;
	bool bGameStarted;
	
	void ResetGame();
	void StartTurn(APlayerController* NewTurnPlayer);
	void OnTurnTimeout();
	void SwitchToNextTurn();
	void JudgeAndBroadcast(APlayerController* Sender, const FString& Guess);
	void CheckWinOrDraw(APlayerController* Sender, int32 Strikes);
	void BroadcastMessage(const FString& Message);
	void SendMessageTargetPlayer(APlayerController* Sender, const FString& Message);
	void NotifyGameStart();
	void UpdateRemainingTime();
};
