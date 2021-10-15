#pragma once
#include "phxpch.h"
#include "discord_rpc.h"
#include "discord_register.h"
#include "Phoenix/Logging/Log.h"

namespace phx {
	class discordRPC {
	public:
		static void SetupDiscord()
		{
			if (gRPC)
			{
				DiscordEventHandlers handlers;
				memset(&handlers, 0, sizeof(handlers));
				Discord_Initialize("878503814603345930", &handlers, 1, "0");
				eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				PHX_CORE_INFO("Discord RPC Initialized");
			}
			else
			{
				Discord_Shutdown();
				return;
			}
		}

		static void UpdateDiscordStatus(char* status)
		{
			discordStatus = status;
			RefreshDiscord();
			PHX_CORE_INFO("Discord status updated to: {0}", discordStatus);
		}

		static void RefreshDiscord()
		{
			DiscordRichPresence discordPresence;
			memset(&discordPresence, 0, sizeof(discordPresence));
			discordPresence.details = ("{0}", discordStatus);
			discordPresence.startTimestamp = eptime;
			discordPresence.largeImageKey = "phoenix_new";
			discordPresence.largeImageText = "Editing On Phoenix Engine";
			discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
			Discord_UpdatePresence(&discordPresence);
		}
	private:
		static int64_t eptime;
		static char* discordStatus;
		static const bool gRPC = true;
	};
}