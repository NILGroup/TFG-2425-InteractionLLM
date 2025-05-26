// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RR_JsonFileManager.h"
#include "RR_ExperimentGroups.generated.h"

USTRUCT(BlueprintType)
struct FExperimentGroupsStruct : public FJsonStruct {

	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int NumOfGroups = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, int> Groups = TMap<FString, int>();
}; 


class RADIATIONROOM_API RR_ExperimentGroups
{
public:
	RR_ExperimentGroups();
	~RR_ExperimentGroups();

};
