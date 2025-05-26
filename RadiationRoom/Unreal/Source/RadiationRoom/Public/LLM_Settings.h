// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LLM_Settings.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FCommand
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere)
	FString CommandArgument = TEXT("");

	UPROPERTY(Config, EditAnywhere)
	FString CommandValue = TEXT("");
};

/*
* Clase que contiene los ajustes del proyecto que se quieran aplicar durante la ejecución del script del LLM
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "LLM Settings"))
class RADIATIONROOM_API ULLM_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	ULLM_Settings();

	UPROPERTY(Config, EditAnywhere, Category = "Commands", BlueprintReadOnly)
	FString scriptName;

	UPROPERTY(Config, EditAnywhere, Category = "Commands", BlueprintReadOnly)
	TArray<FCommand> LLM_Commands;

	UFUNCTION(BlueprintCallable)
	const FString GetSettingsCommands() const;

	UFUNCTION(BlueprintCallable)
	const FString GetArgumentValue(FString argName) const;

	UFUNCTION(BlueprintCallable)
	static const ULLM_Settings* GetLLM_Settings();
};
