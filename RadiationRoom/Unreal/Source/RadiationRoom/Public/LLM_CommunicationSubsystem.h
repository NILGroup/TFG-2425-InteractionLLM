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

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLLMResponseReceived, const FString&, Response);
UCLASS()
class RADIATIONROOM_API ULLM_CommunicationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	ULLM_CommunicationSubsystem();

	UFUNCTION(BlueprintCallable)
	void SendMessage(FString userMessage);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsPendingResponse() { return bPendingResponse; }

	UPROPERTY(BlueprintAssignable)
	FOnLLMResponseReceived OnLLMResponseReceived;
	
protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	void ShowLLMResponse();
private:
	int32 winSockInitialization();
	int32 socketConnection();
	bool bConnectionSuccesful = false;

	// Necesario para poder inicializar el uso del archivo DLL de Windows Sockets
	WSADATA wsaData;
	SOCKET llmSocket = INVALID_SOCKET;
	struct sockaddr_in server_addr;
	bool bPendingResponse = false;
};
