// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

//contiene la mayoría de las funciones, estructuras y definiciones de Winsock.
#include <winsock2.h>
//contiene definiciones introducidas en el documento anexo de WinSock 2 Protocol-Specific para TCP/IP que incluye funciones y estructuras más recientes que se usan para recuperar direcciones IP.
#include <ws2tcpip.h>
#include <stdio.h>
// Asegúrese de que el entorno de compilación se vincula al archivo de biblioteca winsock Ws2_32.lib (Comentario de la guía oficial de cómo usar WinSock)
#pragma comment(lib, "Ws2_32.lib")

#include "STT_CommunicationSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTranscriptionRecieved, const FString&, Response);
UCLASS()
class RADIATIONROOM_API USTT_CommunicationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	USTT_CommunicationSubsystem();

	UPROPERTY(BlueprintAssignable)
	FOnTranscriptionRecieved OnTranscriptionRecieved;
	
	virtual void Tick(float DeltaTime) override;

protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	void RecieveTranscriptionsLoop();


private:
	int32 SystemCall(FString pythonCommand);
	int32 winSockInitialization();
	int32 socketConnection();
	bool bConnectionSuccesful = false;
	
	TStatId GetStatId() const override;

	bool _transcriptionRecieved = false;
	FString _transcription = "";

	// Necesario para poder inicializar el uso del archivo DLL de Windows Sockets
	WSADATA wsaData;
	SOCKET STTsocket = INVALID_SOCKET;
};
