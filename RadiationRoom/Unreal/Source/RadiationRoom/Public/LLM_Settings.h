// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LLM_Settings.generated.h"

USTRUCT()
struct FCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString CommandArgument = TEXT("");

	UPROPERTY(EditAnywhere)
	FString CommandValue = TEXT("");
};

UCLASS(config = LLMSettings)
class RADIATIONROOM_API ULLM_Settings : public UObject
{
	GENERATED_BODY()

public:
	ULLM_Settings(const FObjectInitializer& obj);

	const FString GetSettingsCommands();
protected:
	UPROPERTY(Config, EditAnywhere, Category = "Commands")
	TArray<FCommand> LLM_Commands;

	//Estas variables son los argumentos que se pasan antes de cada valor
	/*UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString ExitCommandArg = TEXT("exit_msg");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString HostPortArg = TEXT("port");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString HostIPArg = TEXT("host_ip");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString HistoryMaxMemoryArg = TEXT("hist_max_mem");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString PerformanceModeArg = TEXT("perf_mode");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Arguments")
	FString ExecutionModeArg = TEXT("exe_mode");

	//Estos son los valores de cada argumento
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values")
	FString ExitCommand = TEXT("LLM_Quit");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values", meta = (ClampMin = "0"))
	int32 HostPort = 8080;

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values")
	FString HostIP = TEXT("127.0.0.1");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values", meta = (ClampMin = "0"))
	int32 HistoryMaxMemory = 0;

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values")
	FString PerformanceMode = TEXT("Fast");

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Values")
	FString ExecutionMode = TEXT("Release");*/
};
