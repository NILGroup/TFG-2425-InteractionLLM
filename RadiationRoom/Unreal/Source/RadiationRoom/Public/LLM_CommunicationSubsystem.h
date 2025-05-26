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
 * Esta clase permite conectar el motor de Unreal al script de Python que ejecuta la lógica del LLM
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLLMResponseReceivedDelegate, const FString&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLLMQuestionSendDelegate);
UCLASS()
class RADIATIONROOM_API ULLM_CommunicationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/// <summary>
	/// Método que permite enviar una solicitud al modelo
	/// </summary>
	/// <param name="userMessage">Mensaje que se quiere enviar</param>
	/// <param name="messagePriority">Prioridad del mensaje, a mayor número, mayor prioridad</param>
	UFUNCTION(BlueprintCallable)
	void SendMessage(FString userMessage, int32 messagePriority = 1);

	/// <summary>
	/// Devuelve si el LLM está todavía procesado una petición anterior
	/// </summary>
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsPendingResponse() { return bPendingResponse; }

	/// <summary>
	/// Event dispatcher para notificar de cuando se recibe una respuesta del LLM
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FOnLLMResponseReceivedDelegate OnLLMResponseReceived; 

	/// <summary>
	/// Event dispatcher que notifica el envío de una nueva petición al LLM
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FOnLLMQuestionSendDelegate OnLLMQuestionSend;

	/// <summary>
	/// Nombre del script a lanzar, el valor se ajusta al valor que aparece en los developer settings
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "settings")
	FString scriptName;
protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	/// <summary>
	/// Método que notifica a todos los objetos suscritos al event dispatcher de la respuesta del LLM
	/// </summary>
	void ShowLLMResponse();

	/// <summary>
	/// Método interno que encola una petición si en el momento de envío ya hay una petición procesándose.
	/// </summary>
	/// <param name="prompt"></param>
	void QueuePrompt(const FPromptInformation& prompt);
	
	/// <summary>
	/// Método que manda la petición al script
	/// </summary>
	/// <param name="prompt"></param>
	void SendPrompt(const FString& prompt);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPromptInformation> promptPQueue;
private:
	/// <summary>
	/// Método que ejecuta el script de Python mediante una llamada al sistema
	/// </summary>
	/// <param name="pythonCommand"></param>
	/// <returns></returns>
	int32 SystemCall(FString pythonCommand);

	/*
	* Métodos y parámetros necesarios para la inicialización y conexión por puertos
	*/
	int32 winSockInitialization();
	int32 socketConnection();
	bool bConnectionSuccesful = false;

	// Necesario para poder inicializar el uso del archivo DLL de Windows Sockets
	WSADATA wsaData;
	SOCKET llmSocket = INVALID_SOCKET;
	struct sockaddr_in server_addr;
	bool bPendingResponse = false;
	FString _llmResponse;
};
