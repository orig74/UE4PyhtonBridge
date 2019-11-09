// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PyServerTickActor.generated.h"

UCLASS()
class MYPROJECT_API APyServerTickActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APyServerTickActor();

	// Called when the game starts or when spawned
protected:
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
public:	
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
