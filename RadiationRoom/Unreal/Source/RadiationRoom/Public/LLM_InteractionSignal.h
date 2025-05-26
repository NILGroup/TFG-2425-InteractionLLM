// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "LLM_InteractionSignal.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSignalAnswer {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString prompt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> affectedActors;
};
/**
 * Clase padre utilizada para la creación de señales hijas.
 * Sirve como "interfaz" para que se implemente la lógica en clases Blueprint derivadas
 */
UCLASS(BlueprintType, Blueprintable)
class RADIATIONROOM_API ULLM_InteractionSignal : public UObject
{
	GENERATED_BODY()

public:
	ULLM_InteractionSignal();

	/// <summary>
	/// Sobreescribir este método permite hacer uso de funciones exclusivas de actores con presencia en el mundo, como por ejemplo, realizar raycasts
	/// </summary>
	/// <returns>Devuelve el mundo en el que se encuentra el objeto</returns>
	UFUNCTION(BlueprintCallable)
	UWorld* GetWorld() const override;

	/// <summary>
	/// Evento de ejecución de la señal, 
	/// </summary>
	/// <returns>True en caso de que se haya podido ejecutar, falso en caso contrario</returns>
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Functions")
	bool ExecuteSignal();

	/// <summary>
	/// Evento que genera el prompt correspondiente, solo se ejecutará este evento si el intento de ejecución ha sido positivo
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Functions")
	FSignalAnswer GetSignalPrompt();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FString description;

	UPROPERTY(BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<class AActor> instigator;
};
