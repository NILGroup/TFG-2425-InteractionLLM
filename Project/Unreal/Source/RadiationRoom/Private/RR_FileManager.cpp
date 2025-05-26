// Fill out your copyright notice in the Description page of Project Settings.


#include "RR_FileManager.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

FString URR_FileManager::ReadFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage) {
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read File Failed - File doesn't exist - '%s'"), *FilePath);
		return "";
	}

	FString RetString = "";
	if (!FFileHelper::LoadFileToString(RetString, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read File Failed - Was not able to read file. Is this a text file? - '%s'"), *FilePath);
		return "";
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read File Succeded - '%s'"), *FilePath);
	return RetString;
}

void URR_FileManager::WriteFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage) {
	if (!FFileHelper::SaveStringToFile(String, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write File Failed - Was not able to write to file. Is your file read only? Is the path valid? - '%s'"), *FilePath);
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write File Succeded - '%s'"), *FilePath);
}
