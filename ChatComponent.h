// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"

class UChatUIWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYNETWORK_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChatComponent();

	UFUNCTION(Server, Reliable)
	void SendChatMessageOnServer(const FString& Message);

	UFUNCTION(Client, Reliable)
	void AddChatMessageOnOwningClient(const FString& Message);

	UFUNCTION(Client, Reliable)
	void AddSystemChatMessageOnOwningClient(const FString& Message);

	UFUNCTION(Server, Reliable)
	void HandlePlayerGuess(const FString& Input);

	UFUNCTION(BlueprintCallable)
	UChatUIWidget* GetChatUI();

protected:
	virtual void BeginPlay() override;

	void EnterPressed();
	void ScrollUp();
	void ScrollDown(); 

private:
	UPROPERTY()
	class UChatBoxWidget* ChatBoxRef;
	UPROPERTY()
	class UChatSystemWidget* ChatSystemRef;
	UPROPERTY()
	class UChatUIWidget* ChatUIRef;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UChatUIWidget> ChatUIWidgetClass;
};
