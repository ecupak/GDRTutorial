// Fill out your copyright notice in the Description page of Project Settings.


#include "FindIP.h"

// To process request and response of HTTP messages.
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


UFindIP* UFindIP::FindIP(UObject* WorldContext)
{
	// Create a new UFindIP instance.
	UFindIP* BlueprintNode = NewObject<UFindIP>();

	return BlueprintNode;
}


void UFindIP::Activate()
{
	// [Credit]
	// https://forums.unrealengine.com/t/39-s-extra-blueprint-nodes-for-you-as-a-plugin-no-c-required/3448/1792

	FHttpModule* HTTPModule = &FHttpModule::Get();
	
	// Confirm module was created and enabled.
	if (!HTTPModule || !HTTPModule->IsHttpEnabled()) {
		OnFailed.Broadcast(); 
		SetReadyToDestroy();
		return;
	}

	// Send HTTP request to get public IP.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HTTPModule->CreateRequest();
	{
		Request->SetVerb("GET");

		Request->SetHeader("Content-Type", "text/html");

		FString TargetURL = "http://api.ipify.org";
		Request->SetURL(TargetURL);

		Request->OnProcessRequestComplete()
			.BindUObject(this, &UFindIP::RequestComplete);
	}

	// Start the request.
	if (!Request->ProcessRequest()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}
}


void UFindIP::RequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check success state.
	if (!bWasSuccessful) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Store IP.
	IPAddress = Response->GetContentAsString();

	// Notify success.	
	OnSucceeded.Broadcast();
	SetReadyToDestroy();
}


FString UFindIP::GetPublicIP()
{
	SetReadyToDestroy();
	return IPAddress;
}