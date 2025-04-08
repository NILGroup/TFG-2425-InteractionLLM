// Fill out your copyright notice in the Description page of Project Settings.

#include "STT_CommunicationSubsystem.h"
#include "LLM_CommunicationSubsystem.h"

#include <string>

#define PORT "7070"
#define HOST "127.0.0.1"
#define WINSOCK_DEPRECATED_NO_WARNINGS

#define RUN_IN_BACKGROUND_COMMAND "start /B " 

USTT_CommunicationSubsystem::USTT_CommunicationSubsystem()
{

}

void USTT_CommunicationSubsystem::SendMessageToSocket(FString message)
{
    FTCHARToUTF8 utf_message(*message);
    int bytesSent = send(STTsocket, utf_message.Get(),message.Len(), 0);
    if (bytesSent != message.Len()) {
        UE_LOG(LogTemp, Error, TEXT("ERROR SENDING PYTHON INPUT"));
        return;
    }
}

/*void USTT_CommunicationSubsystem::Tick(float DeltaTime)
{
    if (_transcriptionRecieved && bConnectionSuccesful) {
        _transcriptionRecieved = false;
        OnTranscriptionRecieved.Broadcast(_transcription);
    }
}*/

void USTT_CommunicationSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
    Super::Initialize(Collection);

    if (SystemCall("py ") != 0) {
        if (SystemCall("python ") != 0) {
            GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("ERROR SYS CALL: STT"));
            return;
        }
    }
    
    int32 ret = winSockInitialization();
    bConnectionSuccesful = (ret == 0);
    if (!bConnectionSuccesful) return;
    
    // Esto hay que cambiarlo por una pantalla de carga hasta que se conecte el socket.
    ret = socketConnection();
    bConnectionSuccesful = (ret == 0);
    while (!bConnectionSuccesful) {
        ret = socketConnection();
        bConnectionSuccesful = (ret == 0);
    }
    if (bConnectionSuccesful) {
        GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, TEXT("Connection Succesful: YES"));
        RecieveTranscriptionsLoop();
    }
    else {
        GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, TEXT("Connection Succesful: NO"));
    }
}

void USTT_CommunicationSubsystem::Deinitialize() {
    Super::Deinitialize();

    if (STTsocket != NULL) {
        closesocket(STTsocket);
    }

    WSACleanup();
}

void USTT_CommunicationSubsystem::RecieveTranscriptionsLoop()
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this] {
        
        int32 iResult = 0;
        // con dos huevazos me hago un while(true)
        while (iResult != SOCKET_ERROR) {
            uint32_t msg_length;
            
            // Recibir longitud del mensaje enviado
            iResult = recv(STTsocket, reinterpret_cast<char*>(&msg_length), sizeof(msg_length), 0);
            if (iResult != SOCKET_ERROR) {
                
                if (iResult == 0) continue;
                else {
                    char* buffer = new char[msg_length + 1];
                    // Recibir mensaje
                    iResult = recv(STTsocket, buffer, msg_length, 0);

                    if (iResult != SOCKET_ERROR)  //En caso de no haber recibido nada o un error de conexión
                    {
                        buffer[msg_length] = '\0';
                        FString res = FString(buffer);
                        if (res == FString("_send_message_")) {
                            // GetWorld()->GetGameInstance()->GetSubsystem<ULLM_CommunicationSubsystem>()->SendMessageW(_transcription, 1);
                            OnTranscriptionEnded.Broadcast(_transcription);
                            _transcription = FString();
                        }
                        if (res == FString("_socket_down_")) {
                            bConnectionSuccesful = false;
                            break;
                        }
                        else {
                            _transcription = FString(buffer);
                            OnTranscriptionRecieved.Broadcast(_transcription);
                            //_transcriptionRecieved = true;
                        }
                        
                    }
                    delete[] buffer;
                }
            }         
        }

        closesocket(STTsocket);
        STTsocket = NULL;
        });
}

int32 USTT_CommunicationSubsystem::SystemCall(FString pythonCommand)
{
    //No hay feedback si el archivo no existe, y si el comando py estuviera mal escrito o python no existiera, saldr�a una ventana de error
    FString PythonScriptPath = pythonCommand + FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + TEXT("Source/") + FApp::GetProjectName() + TEXT("/sttSocket.py"));
    PythonScriptPath = TEXT(RUN_IN_BACKGROUND_COMMAND) + PythonScriptPath;
    std::string ScriptAnsi = TCHAR_TO_UTF8(*PythonScriptPath);
    return system(ScriptAnsi.c_str());
}

int32 USTT_CommunicationSubsystem::winSockInitialization()
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

int32 USTT_CommunicationSubsystem::socketConnection()
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
    STTsocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (STTsocket == INVALID_SOCKET) {
        UE_LOG(LogTemp, Error, TEXT("Error at socket(): %ld\n"), WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return -1;
    }

    iResult = connect(STTsocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(STTsocket);
        STTsocket = INVALID_SOCKET;
        return -1;
    }

    freeaddrinfo(result);

    if (STTsocket == INVALID_SOCKET) {
        UE_LOG(LogTemp, Error, TEXT("Unable to connect to python!\n"));
        WSACleanup();
        return -1;
    }
    return 0;
}

/*TStatId USTT_CommunicationSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(YouClassName, STATGROUP_Tickables);
}*/
