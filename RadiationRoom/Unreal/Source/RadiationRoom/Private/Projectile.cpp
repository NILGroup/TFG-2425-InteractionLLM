// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values for this component's properties
UProjectile::UProjectile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_name = "Projectile";
	_serializedComponent = "{\"speed\":0.0}";
	_description = "It should be added only to objects generated when the instigator is one ranged gun, such as a bullet.";
	// ...
}


// Called when the game starts
void UProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UProjectile::DeserializeComponent(FString jsonObject)
{
	TSharedPtr<FJsonObject> obj;
	auto reader = TJsonReaderFactory<>::Create(jsonObject);
	if (FJsonSerializer::Deserialize(reader, obj)) {
		double speed = 0.0f;
		if (obj->TryGetNumberField("speed", speed)) {
			_speed = speed;
		}
	}
}

