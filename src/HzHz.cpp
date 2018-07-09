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
float val1 = 0.0;
float val2 = 0.0;
	HzHz() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};


void HzHz::step() {

float crush = params[BITCRUSH_AMT].value; //this fades between the 2 crushes
float x = inputs[INPUT1].value;

//val1 = int(x) << int(crush); //bitwise
//can use these other bitwise operators to crush the signal
//char bitw [5] = { &, |, ^, <<, >> };

//knob all the way down
val1 = 10*(x * x); //crazy crush  (you could put the same thing into the "formula module :/")
val1 = clamp(val1, -5.0f, 5.0f); //clamp to 10vpp

//knob all the way up
val2 = 100*(x * x); //crazy crush
val2 = clamp(val2, -5.0f, 5.0f); //clamp to 10vpp

outputs[OUTPUT1].value = val1*(1 - crush) + val2*crush;  //as one value goes up, the other goes down

} //end of step





struct HzHzWidget : ModuleWidget {
	HzHzWidget(HzHz *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/HzHz.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(36.683,201.583), module, HzHz::BITCRUSH_AMT, 0, 1, 0.0));
		addInput(Port::create<PJ301MPort>(Vec(33, 60), Port::INPUT, module, HzHz::INPUT1));
		addOutput(Port::create<PJ301MPort>(Vec(33, 300), Port::OUTPUT, module, HzHz::OUTPUT1));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHzHz = Model::create<HzHz, HzHzWidget>("AmateurResearch", "HzHz", "HzHz", EFFECT_TAG);
