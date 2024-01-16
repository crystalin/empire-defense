#include "sound_common.h"
#include "SoundEngine.h" 
#include "Ammo.h" 
#include "Button.h" 
#include "Party.h" 


void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) <<endl;
        exit(-1);
    }
}


SoundEngine::SoundEngine() : music(NULL), channel(NULL), 
smallClickSound(NULL), bigClickSound(NULL) {
    FMOD_RESULT          result;

	cout << "Loading Music...";
    result = FMOD::System_Create(&system);
    ERRCHECK(result);    
	
    result = system->init(48, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);
	
	result = system->createSound("../../data/musics/welcome_empire.ogg", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &music);
	ERRCHECK(result);

	result = system->createSound("../../data/sounds/big_click.wav", FMOD_HARDWARE, 0, &bigClickSound);
	ERRCHECK(result);

	result = system->createSound("../../data/sounds/small_click2.wav", FMOD_HARDWARE, 0, &smallClickSound);
	ERRCHECK(result);
	
	cout << " done" << endl;
	// result = system->playSound(FMOD_CHANNEL_FREE, music, false, &channel);
    // ERRCHECK(result);

}

SoundEngine::~SoundEngine()
{
    FMOD_RESULT          result;

    result = music->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

}

void SoundEngine::notifyCreateParty(Party* party) {
	party->addObserver(this);
}

void SoundEngine::notifyChangeMap(const Map* map) {
    FMOD_RESULT	result;
	if (channel) {
		result = channel->setPosition(0,FMOD_TIMEUNIT_MS);
		ERRCHECK(result);
	} else {
		result = system->playSound(FMOD_CHANNEL_FREE, music, false, &channel);
		ERRCHECK(result);
	}
}

void SoundEngine::notifyFireAmmo(const Ammo* ammo) {
    /*FMOD::Channel	*channel;
	if (ammo->getType() == TYPE_AMMO_ARROW) {
		system->playSound(FMOD_CHANNEL_FREE, arrowSound, false, &channel);
	} else if (ammo->getType() == TYPE_AMMO_PROJECTILE) {
		system->playSound(FMOD_CHANNEL_FREE, canonSound, false, &channel);
	}*/
}

void SoundEngine::notifyAmmoHit(const Ammo* ammo) {
    /*FMOD::Channel	*channel;
	if (ammo->getType() == TYPE_AMMO_PROJECTILE) {
		system->playSound(FMOD_CHANNEL_FREE, canonExplosionSound, false, &channel);
	}*/
}


void SoundEngine::notifyPressButton(const Button* button) {
    FMOD::Channel	*channel;
	if (button->getSize() >= 64) {
		system->playSound(FMOD_CHANNEL_FREE, bigClickSound, false, &channel);
	} else {
		system->playSound(FMOD_CHANNEL_FREE, smallClickSound, false, &channel);
	}
}

void SoundEngine::notifyReleaseButton(const Button* button) {
    FMOD::Channel	*channel;
	system->playSound(FMOD_CHANNEL_FREE, smallClickSound, false, &channel);
}