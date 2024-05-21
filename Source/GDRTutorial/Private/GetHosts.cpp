// Fill out your copyright notice in the Description page of Project Settings.

#include "GetHosts.h"

// To process request and response of HTTP messages.
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


UGetHosts* UGetHosts::GetHosts(UObject* WorldContext)
{
	// Create a new URegisterHost instance.
	UGetHosts* BlueprintNode = NewObject<UGetHosts>();	

	return BlueprintNode;
}


void UGetHosts::Activate()
{
	FHttpModule* HTTPModule = &FHttpModule::Get();

	// Confirm module was created and enabled.
	if (!HTTPModule || !HTTPModule->IsHttpEnabled()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Send HTTP request to get list of hosts.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HTTPModule->CreateRequest();
	{
		Request->SetVerb("GET");

		Request->SetHeader("Content-Type", "application/json");

		FString TargetURL = "https://turmeric-wob.azurewebsites.net/games";
		Request->SetURL(TargetURL);

		Request->OnProcessRequestComplete()
			.BindUObject(this, &UGetHosts::RequestComplete);
	}

	// Start the request.
	if (!Request->ProcessRequest()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}
}


void UGetHosts::RequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check success state.
	if (!bWasSuccessful || !Response.IsValid()) {
		OnFailed.Broadcast();
		SetReadyToDestroy();
		return;
	}
		
	// Process response.
	// [Credit] http://www.rameezqurashi.com/2020/09/json-parsing-in-unreal-engine-4.html
	
	// Convert response to JSON object.
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	TMap<FString, TSharedPtr<FJsonValue>> JsonValues = ResponseObj->Values;

	FString Message;
	TSharedPtr<FJsonValue>* FieldValue = JsonValues.Find("games");
		
	// Debug.
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Cyan, FString::Printf(TEXT("%s"), FieldValue));

	// Notify success.	
	OnSucceeded.Broadcast();
	SetReadyToDestroy();
}
