#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

volatile bool finished = false;
Mix_Chunk* sound = nullptr;

void finishedCallback(int _ch)
{
    std::cerr << "Music finished!\n";
    if (sound != nullptr)
    {
        Mix_FreeChunk(sound);
    }
    finished = true;
    Mix_CloseAudio();
    SDL_Quit();
    std::cout << "SDL Quit\n";
}

int playSound(std::string _path)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Load WAV file
    sound = Mix_LoadWAV(_path.c_str());
    if (sound == nullptr) {
        std::cerr << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        finished = true;
    } else {
        // Play the sound (channel -1 finds the first available channel, 0 loops once, etc.)
        Mix_ChannelFinished(finishedCallback);
        Mix_PlayChannel(-1, sound, 0);

        // Wait while the sound is playing (simple wait, a real application would use a loop/event system)
        // SDL_Delay(2000); 

        // Free the sound effect
        // Mix_FreeChunk(sound);
    }
    return 0;
}

int main(int argc, char* args[]) {
    int ok = playSound("../r2-d2.mp3");
    while(!finished);

    return 0;
}
