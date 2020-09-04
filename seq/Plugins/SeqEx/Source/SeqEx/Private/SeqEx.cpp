// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SeqEx.h"
#include "SeqExPrivate.h"

#define LOCTEXT_NAMESPACE "FSeqExModule"

void FSeqExModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSeqExModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSeqExModule, SeqEx)



DECLARE_LOG_CATEGORY_EXTERN(SeqExLog, Verbose, All);
DEFINE_LOG_CATEGORY(SeqExLog);

static FString AppendLogPrefix(const UObject* WorldContextObject, const FString& msg)
{
	if (WorldContextObject != nullptr)
	{
		auto World = WorldContextObject->GetWorld();
		FString Prefix;
		if (World && World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			}
		}

		return FString::Printf(TEXT("[%s] %s: %s"), *GetNameSafe(WorldContextObject),
			*Prefix, *msg);
	}
	return msg;
}
extern void SeqExLog_Debug(const FString& msg)
{
	SeqExLog_Debug(nullptr, msg);
}
extern void SeqExLog_Debug(const UObject* WorldContextObject, const FString& msg)
{
	auto msg1 = AppendLogPrefix(WorldContextObject, msg);

	UE_LOG(SeqExLog, Verbose, TEXT("%s"), *msg1);
	// 	const FString& FinalDisplayString = msg;
	// 	GEngine->AddOnScreenDebugMessage((uint64)-1, 5, FColor::Red, FinalDisplayString);
}


extern FString ToString(const FMovieSceneObjectBindingID& ID, IMovieScenePlayer& Player)
{
	for (TWeakObjectPtr<> WeakEventContext : Player.FindBoundObjects(ID.GetGuid(), ID.GetSequenceID()))
	{
		if (UObject* EventContext = WeakEventContext.Get())
		{
			return EventContext->GetFullName();
		}
	}
	return TEXT("None");
}