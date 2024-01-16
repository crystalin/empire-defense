#include "common.h"
#include "model.h"
#include "GraphicEngine.h"
#include "SoundEngine.h"
#include "PCController.h"

Controller* controller = NULL;
GraphicEngine *graphicEngine = NULL;
SoundEngine *soundEngine = NULL;
Model *model = NULL;

int glutStartTime = 0;
int glutEndTime = 0;

void CheckExit (void)
{
	if (model) {
		std::cout << "-----------------------------------" << std::endl;
		//std::cout << "Model Updates : " << model->getTotalUpdates() << std::endl;
		//std::cout << "Model Time Spent : " << model->getTotalTimeSpent() << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		
	}
	if (controller) delete controller;
	if (graphicEngine) delete graphicEngine;
	if (soundEngine) delete soundEngine;
	if (model) delete model;

	#ifdef CHECK_MEMORY
		DumpUnfreed();
	#endif
}


int main(int argc, char **argv) 
{ 
	atexit (CheckExit);
	try
	{
		model = new Model();
		graphicEngine = new GraphicEngine();
		soundEngine = new SoundEngine();
		controller = new PCController(model, graphicEngine);

		graphicEngine->addUIObserver(soundEngine);
		model->addObserver(graphicEngine);
		model->addObserver(soundEngine);
		
		controller->init(argc, argv);
		controller->start();

		delete controller;
		delete graphicEngine;
		delete soundEngine;
		delete model;

	} catch (EmpireException& e) {
        std::cerr << "EmpireException:\n";
       	std::cerr << e.getFullDescription().c_str() << "\n";
		exit(1);
	} catch ( char const *e) {
		std::cerr << "Exception: " << e << "\n";
	}

	return EXIT_SUCCESS; 
}
