#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include "include/discord_rpc.h"

//Handle Discord Ready Event
static void handleDiscordReady(const DiscordUser* u)
{
    std::cout << "\nDiscord: ready" << std::endl;
}

//Handle Discord Disconnected Event
static void handleDiscordDisconnected(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

//Handle Discord Error Event
static void handleDiscordError(int errcode, const char* message)
{
    std::cout << printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

//Shutdown discord-rp
void Shutdown(int sig)
{
    std::cout << "\nShutting Down..." << std::endl;
    Discord_Shutdown();
    exit(sig);
}

//Update Discord Rich Presence
void updatePresence(const char* state, const char* details, long long startTimestamp, long long endTimestamp, const char* smallImageKey,
        const char* smallImageText, const char* largeImageKey, const char* largeImageText)
{
    //Set required variables
    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    //Make sure that state nor details are either too big for discord or not set, then set them
    if (sizeof(state) > 128 || strlen(state) < 1)
    {
        std::cout << "\nState parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown(1);
    }
    discordPresence.state = state;
    if (sizeof(details) > 128 || strlen(details) < 1)
    {
        std::cout << "\nDetails parameter is too long or not set\nPress any key to exit..." << std::endl;
        Shutdown(1); 
    }
    sprintf(buffer, "%s", details);
    discordPresence.details = buffer;
    if (strlen(largeImageKey) < 1)
    {
        std::cout << "\nLargeImage parameter not set\nPress any key to exit..." << std::endl;
        Shutdown(1); 
    }
    discordPresence.largeImageKey = largeImageKey;
    
    if (startTimestamp >= 0) discordPresence.startTimestamp = (int64_t)startTimestamp;
    if (endTimestamp >= 0) discordPresence.endTimestamp = (int64_t)endTimestamp;

    //Make sure not to set the optional variables if they are not defined in the config
    if (!(strlen(smallImageKey) < 1)) discordPresence.smallImageKey = smallImageKey;
    if (!(strlen(smallImageText) < 1)) discordPresence.smallImageText = smallImageText;
    if (!(strlen(largeImageText) < 1)) discordPresence.largeImageText = largeImageText;

    //Actaully update the presence
    Discord_UpdatePresence(&discordPresence);
}

//Initialize Discord Rich Presence
void InitDiscord(const char* clientID)
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.errored = handleDiscordError;
    handlers.disconnected = handleDiscordDisconnected;
    if (strlen(clientID) < 1)
    {
        std::cout << "\nClientID not set" << std::endl;
        Shutdown(1);
    }
    Discord_Initialize(clientID, &handlers, 1, NULL);
}

