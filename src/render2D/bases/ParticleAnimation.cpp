#include "render2D_common.h"
#include "ParticleAnimation.h"
#include "RessourceManager.h"

Texture* ParticleAnimation::particleTexture = NULL;

#define PARTICLE_SIZE 0.3f

ParticleAnimation::ParticleAnimation(int count, float life, int time, int loop, const Rectangle<float>& area, const Point2D<float>& size, float speed, Color color, float xg, float yg, float expansion) : 
Animation(time,loop,area,speed,color), count(count), size(size), life(life), particleLeft(count), xg(xg), yg(yg), expansion(expansion)
{
	if (particleTexture==NULL) {
		particleTexture = RessourceManager::instance()->getTexture("particle.png");
	}
	particleTexture->request();

	particles = new Particle[count];
	for(int i=0; i<count; i++) {
		particles[i].active = true;
		initParticle(particles[i],true);
    }
}

ParticleAnimation::~ParticleAnimation() 
{
	delete[] particles;
	particleTexture = particleTexture->release();
}

void ParticleAnimation::initParticle(Particle& particle, bool start)
{
	if (start) {
		particle.life = myRand(0,1);
	} else {
		particle.life = 1.0f;
	}

    particle.fade = myRand(1/life,1);

	float colorIntensity = myRand(-0.2,0.2);
    particle.r=colorIntensity * color.values[0] + color.values[0] ;
    particle.g=colorIntensity * color.values[1] + color.values[1] ;
    particle.b=colorIntensity * color.values[2] + color.values[2] ;
    particle.a=colorIntensity * color.values[3] + color.values[3] ;

	if (particle.r<0) particle.r = 0;
	if (particle.r>1) particle.r = 1;

	if (particle.g<0) particle.g = 0;
	if (particle.g>1) particle.g = 1;

	if (particle.b<0) particle.b = 0;
	if (particle.b>1) particle.b = 1;

	if (particle.a<0) particle.a = 0;
	if (particle.a>1) particle.a = 1;

    particle.x = myRand(area.x1,area.x2);
    particle.y = myRand(area.y1,area.y2);

    particle.xi = particle.x-(area.x1+area.getSizeX()/2.0f);
    particle.yi = particle.y-(area.y1+area.getSizeY()/2.0f);

	/*while (abs(particle.y*particle.x)<PARTICLE_SIZE/1000.0f) {
		particle.y += particle.y + (particle.y < 0 ? - PARTICLE_SIZE/500.0f : PARTICLE_SIZE/500.0f);
		particle.x += particle.x + (particle.x < 0 ? - PARTICLE_SIZE/500.0f : PARTICLE_SIZE/500.0f);
	}*/

	if (expansion > 0.0f) {
		particle.xg = particle.xi * expansion;
		particle.yg = particle.yi * expansion;
	} else  {
		particle.xg = xg;
		particle.yg = yg;
	}
}

void ParticleAnimation::update(int timeSpent)
{
	if (state == 0) return ;

	timeLeft -= timeSpent*speed;
	/*if (timeLeft <= 0 && loop == 1) {
		timeLeft = (timeLeft % timeBase) + timeBase;
	}*/

	float timeSpentf = timeSpent/1000.0f * speed;
	for(int i=0; i<count; i++) // Pour chaque particule
    {   if(particles[i].active)         // Si elle est active
        {   
            /* Déplacement */
            particles[i].x += particles[i].xi*timeSpentf;
            particles[i].y += particles[i].yi*timeSpentf;

            /* Gravité */
            particles[i].xi += particles[i].xg*timeSpentf;
            particles[i].yi += particles[i].yg*timeSpentf;

            /* "Vie" */
            particles[i].life -= particles[i].fade*timeSpentf;

            /* Si la particule est "morte" on la régénère */
            if (particles[i].life <= 0.0f ) {
				if (timeLeft > 0.0f) {
					initParticle(particles[i]);
				} else {
					particles[i].active = false;
					particleLeft--;
				}
            }
        }
	}

}

bool ParticleAnimation::preDisplay(int addTime)
{
	return true;
}

void ParticleAnimation::postDisplay(int addTime)
{
	particleTexture->bind();

	glPushMatrix();
	for(int i=0; i<count; i++) // Pour chaque particule
    {   if(particles[i].active)         // Si elle est active
        {   
			float x = particles[i].x;   // On récupère sa position
            float y = particles[i].y;

			float fade =  particles[i].a * particles[i].life;

			if (!loop) {
				fade *= (timeLeft/(float)timeBase);
			}
            //glColor4f(particles[i].r+(abs(x)+abs(y))/2.0, particles[i].g+(abs(x)+abs(y))/2.0, particles[i].b+(abs(x)+abs(y)),  fade );
			glColor4f(particles[i].r, particles[i].g, particles[i].b,fade);

			particleTexture->drawSquare(Rectangle<float>(x,y,x+size.x,y+size.y));
        }
	}
	glPopMatrix();
}