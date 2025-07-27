// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSessionSubsystem.h"

#include "Engine/AssetManager.h"

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	FAssetData MapAssetData;
	if ( UAssetManager::Get().GetPrimaryAssetData( MapID, MapAssetData ) )
	{
		return MapAssetData.PackageName.ToString();
	}

	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for ( const auto& ExtraArg : ExtraArgs )
	{
		if ( ExtraArg.Key.IsEmpty() )
		{
			continue;
		}

		if ( ExtraArg.Value.IsEmpty() )
		{
			CombinedExtraArgs += FString::Printf( TEXT( "?%s" ), *ExtraArg.Key );
		}
		else
		{
			CombinedExtraArgs += FString::Printf( TEXT( "?%s=%s" ), *ExtraArg.Key, *ExtraArg.Value );
		}
	}

	return FString::Printf( TEXT( "%s%s" ), *GetMapName(), *CombinedExtraArgs );
}

void UCommonSessionSubsystem::HostSession(APlayerController* InHostingPlayer, UCommonSession_HostSessionRequest* InRequest)
{
	const ULocalPlayer* LocalPlayer = ( InHostingPlayer != nullptr ) ? InHostingPlayer->GetLocalPlayer() : nullptr;
	if ( LocalPlayer == nullptr )
	{
		return;
	}

	GetWorld()->ServerTravel( InRequest->ConstructTravelURL() );
}
