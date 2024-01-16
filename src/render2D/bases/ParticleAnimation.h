#ifndef __PARTICLE_ANIMATION_H__
#define __PARTICLE_ANIMATION_H__

#include "Animation.h"
#include "Texture.h"

typedef struct // Création de la structure
{  bool active; // Active (1=Oui/0=Non)
   float life; // Durée de vie
   float fade; // Vitesse de disparition
   float r, g, b, a; // Valeurs RGB pour la couleur
   float x, y; // Position
   float xi, yi; // Vecteur de déplacement
   float xg, yg; // Gravité
}Particle;

class ParticleAnimation : public Animation
{
public:
	ParticleAnimation(int count, float life = 1.0f, int time = 1000, int loop = 0, const Rectangle<float>& area = Rectangle<float>(-32,-32,32,32), const Point2D<float>& size = Point2D<float>(32,32), float speed = 1.0f, Color color = Color(0.8f, 0.2f, 0.2f, 0.9f), float xg = 0.0f, float yg = 0.0f, float expansion = 0.0f );
	virtual ~ParticleAnimation();

	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
	virtual bool isFinished() { return particleLeft==0; }
protected:

	Particle* particles;
	int count;
	int particleLeft;
	float life;
	float expansion;
	Point2D<float> size;
	float xg, yg;

	static Texture* particleTexture;

	void initParticle(Particle& particle, bool start = false);

};

#endif
