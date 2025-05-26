// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RR_FileManager.generated.h"

/**
 * 
 */
UCLASS()
class RADIATIONROOM_API URR_FileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, category = "RR File Manager")
	static FString ReadFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, category = "RR File Manager")
	static void WriteFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage);
	
};
