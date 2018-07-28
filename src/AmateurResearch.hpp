#include "rack.hpp"


using namespace rack;

// Forward-declare the Plugin, defined in Template.cpp
extern Plugin *plugin;

// Forward-declare each Model, defined in each module source file
extern Model *modelHalfWave;
extern Model *modelRectify;
extern Model *modelHzHz;
extern Model *modelEvilVCA;
extern Model *modelEvilMixer;
extern Model *modelGraffiti;
//extern Model *modelSign;


// Testing custom component
struct MixerKnobWhite : SVGKnob {
	MixerKnobWhite() {
        minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		setSVG( SVG::load(assetPlugin(plugin, "res/MixerKnobWhite.svg")) );
	}
};
