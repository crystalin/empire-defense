#ifndef __SOUND_ENGINE__
#define __SOUND_ENGINE__

#include "Observer.h"
#include "UIObserver.h"
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>


class EDSOUND_API SoundEngine : public Observer, public UIObserver
{
public:
	SoundEngine();
	~SoundEngine(void);
	
	virtual void		notifyChangeMap(const Map* map);
	virtual void		notifyCreateParty(Party* party);
	virtual void		notifyFireAmmo(const Ammo* ammo);
	virtual void		notifyAmmoHit(const Ammo* ammo);

	virtual void		notifyPressButton(const Button* button);
	virtual void		notifyReleaseButton(const Button* button);

protected:
	FMOD::System	*system;
    FMOD::Sound		*music;
    FMOD::Channel	*channel;

    FMOD::Sound		*bigClickSound;
    FMOD::Sound		*smallClickSound;
};

#endif
