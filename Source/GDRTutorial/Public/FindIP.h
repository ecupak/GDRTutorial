// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Core
#include "Delegates/DelegateCombinations.h"

// Engine
#include "Kismet/BlueprintAsyncActionBase.h"

// To get HTTP requests.
#include "HttpModule.h"

// Final import.
#include "FindIP.generated.h"


// Delegate used to create all the output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFindIPOutputPin);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncAction))
class GDRTUTORIAL_API UFindIP : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	

public:
	// Sends an HTTP GET request to find the user's public IP.
	UFUNCTION(
		BlueprintCallable, 
		meta = (WorldContext = "WorldContext", BlueprintInternalUseOnly = "true"), 
		Category = "IP Utilities"
	)
	static UFindIP* FindIP(UObject* WorldContext);


	// Output pin when IP is obtained.
	UPROPERTY(
		BlueprintAssignable, 
		Category = "IP Utilities"
	)
	FFindIPOutputPin OnSucceeded;


	// Output pin when IP is not obtained (HTTP request failed, most likely).
	UPROPERTY(
		BlueprintAssignable,
		Category = "IP Utilities"
	)
	FFindIPOutputPin OnFailed;


private:
	// Start the Action (automatically called when Blueprint node is called).
	virtual void Activate() override;


	// Handle result of successful GET request.
	void RequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


public:
	UFUNCTION(
		BlueprintCallable, 
		Category = "IP Utilities"
	)
	FString GetPublicIP();


private:	
	// The found IP address.	
	FString IPAddress = "";
};
