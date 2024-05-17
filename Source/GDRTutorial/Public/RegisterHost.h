// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Core
#include "Delegates/DelegateCombinations.h"

// Engine
#include "Kismet/BlueprintAsyncActionBase.h"

// To get HTTP requests.
#include "HttpModule.h"

// Final import.
#include "RegisterHost.generated.h"


// Delegate used to create all the output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRegisterHostOutputPin);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncAction))
class GDRTUTORIAL_API URegisterHost : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	// Sends an HTTP POST request to register as a listen server.
	UFUNCTION(
		BlueprintCallable,
		meta = (WorldContext = "WorldContext", BlueprintInternalUseOnly = "true"),
		Category = "IP Utilities"
	)
	static URegisterHost* RegisterHost(UObject* WorldContext, FString HostData);


	// Output pin when IP is obtained.
	UPROPERTY(
		BlueprintAssignable,
		Category = "IP Utilities"
	)
	FRegisterHostOutputPin OnSucceeded;


	// Output pin when IP is not obtained (HTTP request failed, most likely).
	UPROPERTY(
		BlueprintAssignable,
		Category = "IP Utilities"
	)
	FRegisterHostOutputPin OnFailed;


private:
	// Start the Action (automatically called when Blueprint node is called).
	virtual void Activate() override;


	// Handle result of successful POST request.
	void RequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	FString HostData = "";
};
