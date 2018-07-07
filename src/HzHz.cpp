#include "AmateurResearch.hpp"


struct HzHz : Module {
	enum ParamIds {
		BITCRUSH_AMT, RECT,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT1,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT1,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

float phase = 1.0;
float newout1 = 0;
float newout2 = 0;
float newout3 = 0;
int shifted = 0;
int shifted2 = 0;
float val = 0.0;

	HzHz() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};

//
//can use these other bitwise operators to crush the signal
//char bitw [5] = { &, |, ^, <<, >> };


void HzHz::step() {

float crush = params[BITCRUSH_AMT].value;

// was trying to make it shift bits one way when knob was turned halfway and shift the other way when turned the other half, but not working
if (crush < 2.5){
outputs[OUTPUT1].value = (int)inputs[INPUT1].value << int(crush);
} else {

//outputs[OUTPUT1].value = (int)inputs[INPUT1].value & int(crush);
float x = inputs[INPUT1].value;
val = 100*(x * x);
outputs[OUTPUT1].value = clamp(val, -5.0f, 5.0f); //clamp output to 10vpp
}
}

struct HzHzWidget : ModuleWidget {
	HzHzWidget(HzHz *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/HzHz.svg")));

//just the screws in the module
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(36.683,201.583), module, HzHz::BITCRUSH_AMT, 0, 5, 0.0));
		addInput(Port::create<PJ301MPort>(Vec(33, 60), Port::INPUT, module, HzHz::INPUT1));
		addOutput(Port::create<PJ301MPort>(Vec(33, 300), Port::OUTPUT, module, HzHz::OUTPUT1));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHzHz = Model::create<HzHz, HzHzWidget>("AmateurResearch", "HzHz", "HzHz", EFFECT_TAG);
