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
	p->addModel(modelGraffiti);
	p->addModel(modelSign);
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
