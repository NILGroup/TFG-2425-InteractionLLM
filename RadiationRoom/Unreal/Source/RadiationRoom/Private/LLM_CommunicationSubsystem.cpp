// Fill out your copyright notice in the Description page of Project Settings.


#include "LLM_CommunicationSubsystem.h"

#define PORT "8080"
#define HOST "127.0.0.1"
#define EXIT_MESSAGE "quit_app"
#define WINSOCK_DEPRECATED_NO_WARNINGS

ULLM_CommunicationSubsystem::ULLM_CommunicationSubsystem()
{

}

void ULLM_CommunicationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    int32 ret = winSockInitialization();
    bConnectionSuccesful = (ret == 0) ? true : false;
    if (!bConnectionSuccesful) return;
    ret = socketConnection();
    bConnectionSuccesful = (ret == 0) ? true : false;
}

void ULLM_CommunicationSubsystem::Deinitialize()
{
    closesocket(llmSocket);
    WSACleanup();
}

int32 ULLM_CommunicationSubsystem::winSockInitialization()
{
    int32 iResult;
    // Inicialización de winSock
    // Se llama a la función WSAStartup para iniciar el uso de WS2_32.dll.
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //El parámetro MAKEWORD(2,2) realiza una solicitud para la versión 2.2 de Winsock en el sistema
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    return 0;
}

int32 ULLM_CommunicationSubsystem::socketConnection()
{
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int32 iResult = getaddrinfo(HOST, PORT, &hints, &result);
    if (iResult != 0) {
        UE_LOG(LogTemp, Error, TEXT("getaddrinfo failed"));
        WSACleanup();
        return -1;
    }
    ptr = result;
    // Creamos el socket con el que conectarse 
    llmSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (llmSocket == INVALID_SOCKET) {
        UE_LOG(LogTemp, Error, TEXT("Error at socket(): %ld\n"), WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return -1;
    }

    // Conexión con el socket de python
    iResult = connect(llmSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(llmSocket);
        llmSocket = INVALID_SOCKET;
        return -1;
    }

    freeaddrinfo(result);

    if (llmSocket == INVALID_SOCKET) {
        UE_LOG(LogTemp, Error, TEXT("Unable to connect to python!\n"));
        WSACleanup();
        return -1;
    }
    return 0;
}

void ULLM_CommunicationSubsystem::SendMessage(FString userMessage)
{
    if (!bConnectionSuccesful) return;
    int iResult;
    std::string message = std::string(TCHAR_TO_UTF8(*userMessage));
    uint32_t messageLen = message.length();
    iResult = send(llmSocket, reinterpret_cast<char*>(&messageLen), sizeof(messageLen), 0); // Enviar longitud
    if (iResult != SOCKET_ERROR) {
        iResult = send(llmSocket, message.c_str(), messageLen, 0); // Enviar mensaje
    }
    if (iResult == SOCKET_ERROR) {
        UE_LOG(LogTemp, Error, TEXT("send failed: %d\n"), WSAGetLastError());
        closesocket(llmSocket);
        WSACleanup();
        bConnectionSuccesful = false;
        return;
    }

    // Recibir respuesta del servidor
    FString messageResponse = RecieveLLMResponse();
    GEngine->AddOnScreenDebugMessage(1, 100, FColor::Green, TEXT("Respuesta:"));
    GEngine->AddOnScreenDebugMessage(2, 100, FColor::Green, messageResponse);
}

FString ULLM_CommunicationSubsystem::RecieveLLMResponse()
{
    uint32_t msg_length;
    recv(llmSocket, reinterpret_cast<char*>(&msg_length), sizeof(msg_length), 0); // Recibir longitud
    char* buffer = new char[msg_length + 1];
    int32 iResult = recv(llmSocket, buffer, msg_length, 0); // Recibir mensaje
    if (iResult == 0)  //En caso de no haber recibido nada
    {
        return FString();
    }
    buffer[msg_length] = '\0';
    FString response(buffer);
    delete[] buffer;
    return response;
}


