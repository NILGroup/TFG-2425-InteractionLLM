// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

// Sets default values for this component's properties
UVehicleController::UVehicleController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_name = "VehicleController";
	_serializedComponent = "{\"color\":0xffff0000}";

	// ...
}


// Called when the game starts
void UVehicleController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVehicleController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleController::DeserializeComponent(FString jsonObject)
{
	TSharedPtr<FJsonObject> obj;
	auto reader = TJsonReaderFactory<>::Create(jsonObject);
	if (FJsonSerializer::Deserialize(reader, obj)) {
		const TSharedPtr<FJsonObject>* ptr;
		if (obj->TryGetObjectField("additional_info", ptr)) {
			uint32 color = 0x00000000;
			(*ptr)->TryGetNumberField("color", color);
			_color = color;
		}
	}
}

