// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintFunctionUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"

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

TMap<FString, TSoftObjectPtr<UObject>> UBlueprintFunctionUtilities::GetObjectsInPath(const FString& DirectoryPath)
{
    TArray<FAssetData> AssetList;
    FARFilter Filter;
    //Filter.ClassNames.Add(classToFind->StaticClass()->GetFName());
    FString dir = "/Game/" + DirectoryPath;
    Filter.PackagePaths.Add(*dir);
    Filter.bRecursivePaths = true;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistryModule.Get().GetAssets(Filter, AssetList);

    TMap<FString, TSoftObjectPtr<UObject>> objsPaths;
    for (const FAssetData& AssetData : AssetList)
    {
        objsPaths.Emplace(AssetData.AssetName.ToString(), AssetData.ToSoftObjectPath());
    }
    return objsPaths;
}


