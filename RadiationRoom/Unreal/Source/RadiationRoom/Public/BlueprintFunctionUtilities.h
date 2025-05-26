// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionUtilities.generated.h"

/**
 * 
 */
UCLASS()
class RADIATIONROOM_API UBlueprintFunctionUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetDirectoriesInDirectory(const FString& DirectoryPath);

	UFUNCTION(BlueprintCallable)
	static TMap<FString, TSoftObjectPtr<UObject>> GetObjectsInPath(const FString& DirectoryPath);
};
