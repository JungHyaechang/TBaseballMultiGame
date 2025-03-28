// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatUIWidget.generated.h"

class UChatBoxWidget;
class UChatSystemWidget;
class UTextBlock;

UCLASS()
class MULTIPLAYNETWORK_API UChatUIWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	UChatBoxWidget* GetChatBox();

	UFUNCTION(BlueprintCallable)
	UChatSystemWidget* GetChatSystem();

	UFUNCTION(BlueprintCallable)
	UTextBlock* GetTurnTimerText();

	UFUNCTION(BlueprintCallable)
	void UpdateAllWinText();

protected:
	UPROPERTY(meta = (BindWidget))
	UChatBoxWidget* ChatBoxWidget;

	UPROPERTY(meta = (BindWidget))
	UChatSystemWidget* ChatSystemWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnTimerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AllPlayerWinsText;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
