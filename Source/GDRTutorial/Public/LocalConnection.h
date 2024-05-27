// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "LocalConnection.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GDRTUTORIAL_API ULocalConnection : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static FString GetLocalIPAndPort(APlayerController* Controller);
};
