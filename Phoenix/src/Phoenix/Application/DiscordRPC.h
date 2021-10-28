#pragma once
#include "phxpch.h"
#include "../vendor/Discord/include/discord_rpc.h"
#include "../vendor/Discord/include/discord_register.h"
namespace phx {
	struct DiscordSpec {
		char* AppID;
		char* State = "";
		char* Status = "";
		char* LargeImageKey = "";
		char* LargeImageText = "";
		char* SmallImageKey = "";
		char* SmallImageText = "";
		char* PartyID = "";
		int PartySize = 0;
		int PartyMax = 0;
		char* JoinSecret;
	};
	
	static int64_t eptime;
	static DiscordSpec CurrentDiscordSpec;

	class DiscordRPC {
	public:
		DiscordRPC() 
		{
			
		}
		
		~DiscordRPC() 
		{
			Shutdown();
		}

		static void SetupDiscord()
		{
			DiscordEventHandlers handlers;

			memset(&handlers, 0, sizeof(handlers));
			Discord_Initialize(CurrentDiscordSpec.AppID, &handlers, 1, "0");

			eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			PHX_CORE_INFO("Discord RPC Initialized");		
		}

		static void RefreshDiscord()
		{
			DiscordRichPresence discordPresence;
			memset(&discordPresence, 0, sizeof(discordPresence));

			discordPresence.state = CurrentDiscordSpec.State;
			discordPresence.details = CurrentDiscordSpec.Status;
			discordPresence.startTimestamp = eptime;
			discordPresence.largeImageKey = CurrentDiscordSpec.LargeImageKey;
			discordPresence.largeImageText = CurrentDiscordSpec.LargeImageText;
			discordPresence.smallImageKey = CurrentDiscordSpec.SmallImageKey;
			discordPresence.smallImageText = CurrentDiscordSpec.SmallImageText;
			discordPresence.partyId = CurrentDiscordSpec.PartyID;
			discordPresence.partySize = CurrentDiscordSpec.PartySize;
			discordPresence.partyMax = CurrentDiscordSpec.PartyMax;
			discordPresence.joinSecret = CurrentDiscordSpec.JoinSecret;

			Discord_UpdatePresence(&discordPresence);
		}

		static Ref<DiscordRPC> Create(char* appID) 
		{
			CurrentDiscordSpec.AppID = appID;
			SetupDiscord();
			return CreateRef<DiscordRPC>();
		}
		static void Shutdown()
		{
			Discord_Shutdown();
		}


	};
}