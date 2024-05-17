// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterHost.h"

// To process request and response of HTTP messages.
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


URegisterHost* URegisterHost::RegisterHost(UObject* WorldContext, FString HostData)
{
	// Create a new URegisterHost instance.
	URegisterHost* BlueprintNode = NewObject<URegisterHost>();
	BlueprintNode->HostData = HostData;

	return BlueprintNode;
}


void URegisterHost::Activate()
{
	FHttpModule* HTTPModule = &FHttpModule::Get();

	// Confirm module was created and enabled.
	if (!HTTPModule || !HTTPModule->IsHttpEnabled()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Send HTTP request to register as host.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HTTPModule->CreateRequest();
	{
		Request->SetVerb("POST");

		Request->SetHeader("Content-Type", "application/json");

		Request->SetContentAsString(HostData);

		FString TargetURL = "https://turmeric-wob.azurewebsites.net/games";
		Request->SetURL(TargetURL);

		Request->OnProcessRequestComplete()
			.BindUObject(this, &URegisterHost::RequestComplete);
	}

	// Start the request.
	if (!Request->ProcessRequest()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}
}


void URegisterHost::RequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check success state.
	if (!bWasSuccessful) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Notify success.	
	OnSucceeded.Broadcast();
	SetReadyToDestroy();
}