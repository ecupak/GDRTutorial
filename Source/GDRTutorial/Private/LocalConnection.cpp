// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalConnection.h"


FString ULocalConnection::GetLocalIPAndPort(APlayerController* Controller) {
	if (Controller) {
		if (Controller->GetNetConnection()) {
			return Controller->GetNetConnection()->RemoteAddressToString();
		}
		else {
			return FString(TEXT("No net connection."));
		}
	}
	else {
		return FString(TEXT("No controller."));
	}
}