// Fill out your copyright notice in the Description page of Project Settings.

#include "RR_JsonFileManager.h"
#include "RR_FileManager.h"

#include "Serialization/JsonSerializer.h" // Json
#include "JsonObjectConverter.h" // JsonUtilities

FJsonStruct URR_JsonFileManager::ReadStructFromJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to read generic json object from file
	TSharedPtr<FJsonObject> JsonObject = ReadJsonFile(FilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return FJsonStruct();
	}

	// Try to convert generic json object to the desired structure. Output goes in RetJsonTestStruct
	FJsonStruct RetJsonTestStruct;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FJsonStruct>(JsonObject.ToSharedRef(), &RetJsonTestStruct))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Struct Json File Failed - Was not able to convert json object to your desired structure. Is it the right format / struct? - '%s'"), *FilePath);
		return FJsonStruct();
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Struct Json File Succeded - '%s'"), *FilePath);
	return RetJsonTestStruct;
}

void URR_JsonFileManager::WriteStructToJsonFile(FString FilePath, FJsonStruct Struct, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (JsonObject == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Struct Json File Failed - Was not able to convert the struct to a json object. This shouldn't really happen."));
		return;
	}

	// Try to write json to file
	WriteJsonFile(FilePath, JsonObject, bOutSuccess, OutInfoMessage);
}




TSharedPtr<FJsonObject> URR_JsonFileManager::ReadJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to read file
	FString JsonString = URR_FileManager::ReadFile(FilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess) 
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> RetJsonObject;

	// Try to convert string to json object. Output goes in RetJsonObject
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject)) 
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json File Failed - Was not able to deserialize json string. Is it the right format? - '%s'"), *JsonString);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json File Succeded - '%s'"), *FilePath);
	return RetJsonObject;

}

void URR_JsonFileManager::WriteJsonFile(FString FilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString;

	// Try to convert json object to string. Output goes in JsonString
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json File Failed - Was not able to serialize the json to string. Is the JsonObject valid?"));
		return;
	}

	// Try to write json string to file
	URR_FileManager::WriteFile(FilePath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json File Succeded - '%s'"), *FilePath);
}
