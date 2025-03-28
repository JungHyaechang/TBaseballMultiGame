// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatEntryWidget.generated.h"


UCLASS()
class MULTIPLAYNETWORK_API UChatEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat", meta = (ExposeOnSpawn = "true"))
	FString InMessage;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;
};
	

