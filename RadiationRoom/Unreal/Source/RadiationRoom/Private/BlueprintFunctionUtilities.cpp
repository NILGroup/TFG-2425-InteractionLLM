// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionUtilities.h"

TArray<FString> UBlueprintFunctionUtilities::GetDirectoriesInDirectory(const FString& DirectoryPath) {
	TArray<FString> files;
	IFileManager& fileManager = IFileManager::Get();
	FString fullPath = FPaths::ProjectContentDir() + DirectoryPath;
	fileManager.IterateDirectory(*fullPath, [&files](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
        {
            if (bIsDirectory)
            {
                files.Add(FPaths::GetCleanFilename(FilenameOrDirectory));
            }
            return true;
        });
	return files;
}
