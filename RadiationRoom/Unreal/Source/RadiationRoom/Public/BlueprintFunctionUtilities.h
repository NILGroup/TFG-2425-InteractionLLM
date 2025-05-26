// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionUtilities.generated.h"

/**
 * Clase encargada de ofrecer las funciones que faciliten el desarrollo 
 */
UCLASS()
class RADIATIONROOM_API UBlueprintFunctionUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Este método, permite obtener todos los recursos dada un directorio concreto
	/// </summary>
	/// <param name="DirectoryPath">Directorio donde se quiere buscar</param>
	/// <returns>Mapa que contiene los nombres de los recursos y referencias a los mismos</returns>
	UFUNCTION(BlueprintCallable)
	static TMap<FString, TSoftObjectPtr<UObject>> GetObjectsInPath(const FString& DirectoryPath);
};
