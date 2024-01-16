#ifndef __SOUND_COMMON_H__
#define __SOUND_COMMON_H__

#ifdef EDSOUND_EXPORTS
#define EDSOUND_API __declspec(dllexport)
#else
#define EDSOUND_API __declspec(dllimport)
#endif

#include "common.h"

#endif