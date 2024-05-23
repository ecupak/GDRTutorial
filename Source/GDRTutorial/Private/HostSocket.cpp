// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSocket.h"
#include <string>


AHostSocket::AHostSocket()
{
	PrimaryActorTick.bCanEverTick = true;
	Socket = nullptr;
}


AHostSocket::~AHostSocket() { }


void AHostSocket::BeginPlay()
{
	Super::BeginPlay();

	SocketSubsystem = nullptr;

	if (SocketSubsystem == nullptr) {
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}

	SendSize = 2 * 1024 * 1024;
	BufferSize = 2 * 1024 * 1024;

	LocalEndpoint = FIPv4Endpoint(FIPv4Address::Any, LocalPort);

	FIPv4Address::Parse(IP, RemoteAddress);
	RemoteEndpoint = FIPv4Endpoint(RemoteAddress, RemotePort);

	Socket = nullptr;

	if (SocketSubsystem != nullptr)
	{
		if (Socket == nullptr)
		{
			Socket = FUdpSocketBuilder(SocketDescription)
				.AsNonBlocking()
				.AsReusable()
				.BoundToEndpoint(LocalEndpoint)
				.WithReceiveBufferSize(SendSize)
				.WithReceiveBufferSize(BufferSize)
				.WithBroadcast();
		}
	}
}


void AHostSocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SocketSubsystem->DestroySocket(Socket);
	Socket = nullptr;
	SocketSubsystem = nullptr;
}


void AHostSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Listen(); // Listen for messages
}


void AHostSocket::Listen()
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	uint32 Size;

	while (Socket->HasPendingData(Size))
	{
		int32 BytesRead = 0;

		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		Socket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);

		char ansiiData[1024];
		memcpy(ansiiData, ReceivedData.GetData(), BytesRead);
		ansiiData[BytesRead] = 0;

		FString data = ANSI_TO_TCHAR(ansiiData);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Message by UDP: " + data);
	}
}


bool AHostSocket::SendMessage(FString Message)
{
	if (!Socket) return false;

	int32 BytesSent;

	FTimespan waitTime = FTimespan(10);

	TCHAR* serializedChar = Message.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);

	bool success = Socket->SendTo(
		(uint8*)TCHAR_TO_UTF8(serializedChar), 
		size, 
		BytesSent, 
		*RemoteEndpoint.ToInternetAddr()
	);

	UE_LOG(LogTemp, Warning, 
		TEXT("Sent message: %s : %s : Address - %s : BytesSent - %d"), 
		*Message, 
		success ? TEXT("true") : TEXT("false"), 
		*RemoteEndpoint.ToString(), 
		BytesSent
	);

	return (success && BytesSent > 0);
}