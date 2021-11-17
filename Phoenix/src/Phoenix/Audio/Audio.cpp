#include "Audio.h"
#include "Phoenix/Logging/Log.h"
#include <al.h>

namespace phx {
    Audio::~Audio()
    {
        delete m_Device;
        delete m_Context;
    }
    void Audio::Init()
	{ 
        
	}
    bool Audio::CheckAlErrors()
    {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            switch (error)
            {
            case AL_INVALID_NAME:
                PHX_CORE_ERROR("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
                break;
            case AL_INVALID_ENUM:
                PHX_CORE_ERROR("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
                break;
            case AL_INVALID_VALUE:
                PHX_CORE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                break;
            case AL_INVALID_OPERATION:
                PHX_CORE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
                break;
            case AL_OUT_OF_MEMORY:
                PHX_CORE_ERROR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
                break;
            default:
                PHX_CORE_ERROR("UNKNOWN AL ERROR: {0}", error);
            }
            return false;
        }
        return true;
    }
}