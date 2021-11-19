#include "phxpch.h"

#include "Audio.h"

#include "Phoenix/Logging/Log.h"

#include <AL/al.h>

namespace phx {
    Audio::Audio()
    {
    }
    Audio::~Audio()
    {
    }
    uint32_t Audio::CreateSound(std::filesystem::path path)
    {
        uint32_t sound = SoundBuffer::New()->AddSoundEffect(path);
        return sound;
    }
}