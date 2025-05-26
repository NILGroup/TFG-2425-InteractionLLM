// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RR_JsonFileManager.generated.h"

class FJsonObject;

USTRUCT(BlueprintType, category = "RR Json File Manager")
struct RADIATIONROOM_API FJsonStruct {

	GENERATED_BODY()
/*
// These variables are visible in both Blueprint and is in Json
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MyString = "Llama";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool MyBool = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MyFloat = 123.456f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MyInt = 123;

// These variables are not visible in Blueprint, but are still visible in Json
public:
	UPROPERTY()
	FVector MyVector = { 1.0f, 2.0f, 3.0f };
	UPROPERTY()
	FRotator MyRotator = { 90.0f, 180.0f, 270.0f };
	UPROPERTY()
	FTransform MyTransform;

// This variable is nos visible in Blueprint or Json
public:
	FString MyOtherString = "This variable will not be in the json because it's not flagged as UPROPERTY()";
	*/
};


UCLASS()
class RADIATIONROOM_API URR_JsonFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RR Json File Manger")
	static FJsonStruct ReadStructFromJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, category = "RR Json File Manager")
	static void WriteStructToJsonFile(FString FilePath, FJsonStruct Struct, bool& bOutSuccess, FString& OutInfoMessage);


public:

	//UFUNCTION(BlueprintCallable, category = "RR Json File Manager")
	static TSharedPtr<FJsonObject> ReadJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);

	//UFUNCTION(BlueprintCallable, category = "RR Json File Manager")
	static void WriteJsonFile(FString FilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage);
	
};
