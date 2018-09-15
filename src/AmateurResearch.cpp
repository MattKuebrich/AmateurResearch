#include "AmateurResearch.hpp"


Plugin *plugin;


void init(Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);

	// Add all Models defined throughout the plugin
	p->addModel(modelHalfWave);
	p->addModel(modelRectify);
	p->addModel(modelHzHz);
	p->addModel(modelEvilVCA);
	p->addModel(modelEvilMixer);
	p->addModel(modelGraffiti);
	p->addModel(modelSign);
	p->addModel(modelSign2);
	p->addModel(modelGreb);
	p->addModel(modelDelayThing);
}
