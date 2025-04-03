// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <string>
#include "Async/Async.h"
//contiene la mayoría de las funciones, estructuras y definiciones de Winsock.
#include <winsock2.h>
//contiene definiciones introducidas en el documento anexo de WinSock 2 Protocol-Specific para TCP/IP que incluye funciones y estructuras más recientes que se usan para recuperar direcciones IP.
#include <ws2tcpip.h>
#include <stdio.h>
// Asegúrese de que el entorno de compilación se vincula al archivo de biblioteca winsock Ws2_32.lib (Comentario de la guía oficial de cómo usar WinSock)
#pragma comment(lib, "Ws2_32.lib")

#include "LLM_CommunicationSubsystem.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "La estructura guarda la información de una prompt que se encola por prioridad"))
struct FPromptInformation {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString promptText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 promptPriority;
};
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLLMResponseReceivedDelegate, const FString&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLLMQuestionSendDelegate);
UCLASS()
class RADIATIONROOM_API ULLM_CommunicationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	ULLM_CommunicationSubsystem();

	UFUNCTION(BlueprintCallable)
	void SendMessage(FString userMessage, int32 messagePriority = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsPendingResponse() { return bPendingResponse; }

	UPROPERTY(BlueprintAssignable)
	FOnLLMResponseReceivedDelegate OnLLMResponseReceived; 

	UPROPERTY(BlueprintAssignable)
	FOnLLMQuestionSendDelegate OnLLMQuestionSend;

	virtual void Tick(float DeltaTime) override;
	
protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	void ShowLLMResponse();

	void QueuePrompt(const FPromptInformation& prompt);

	void SendPrompt(const FString& prompt);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPromptInformation> promptPQueue;
private:
	int32 SystemCall(FString pythonCommand);
	int32 winSockInitialization();
	int32 socketConnection();
	bool bConnectionSuccesful = false;

	TStatId GetStatId() const override;

	// Necesario para poder inicializar el uso del archivo DLL de Windows Sockets
	WSADATA wsaData;
	SOCKET llmSocket = INVALID_SOCKET;
	struct sockaddr_in server_addr;
	bool bPendingResponse = false;
	bool _responseReceived = false;
	FString _llmResponse;
};
