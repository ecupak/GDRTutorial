// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "HostSocket.generated.h"


UCLASS()
class GDRTUTORIAL_API AHostSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHostSocket();
	~AHostSocket();

	FSocket* Socket = nullptr;

	// Local Endpoint
	FString SocketDescription = "UDP Listen Socket";
	FIPv4Endpoint LocalEndpoint;
	uint16 LocalPort = 7777;
	int32 SendSize = 0;
	TArray<uint8> ReceivedData;

	// Remote Endpoint
	FIPv4Endpoint RemoteEndpoint;
	FIPv4Address RemoteAddress;
	uint16 RemotePort = 55895;
	int32 BufferSize = 0;
	//FString IP = "192.168.50.232";
	FString IP = "127.0.0.1";

	ISocketSubsystem* SocketSubsystem = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	void Listen();

	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
	bool SendMessage(FString Message);
};