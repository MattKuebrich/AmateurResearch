// HALFWAVE
// right now this takes 2 input signals and takes the top half from one and the bottom half from the other.
// it works but it's not that interesting... it sounds nearly the same as if they just went through a mixer
// the switch switches the order and the jack next to it sort of modulates it (just turns it ON or OFF)..

#include "AmateurResearch.hpp"


struct HalfWave : Module {
	enum ParamIds {
		PITCH_PARAM, INVERT,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT1,INPUT2,INVERT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT1,OUTPUT2,OUTPUT3,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,BLINK_LIGHT2,
		NUM_LIGHTS
	};

float phase = 1.0;
float out_top = 0;
float out_bottom = 0;


	HalfWave() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};



void HalfWave::step() {
float halfer = inputs[INVERT_INPUT].value;

if (params[INVERT].value == 0){
	if (inputs[INPUT1].value < halfer) {
		out_top = inputs[INPUT1].value;
	} else {
	out_top = 0;
	}

} else {
	if (inputs[INPUT1].value > halfer) {
		out_top = inputs[INPUT1].value;
	} else {
	out_top = 0;
	}
}


if (params[INVERT].value == 1){
	if (inputs[INPUT2].value < halfer) {
		out_bottom = inputs[INPUT2].value;
	} else {
	out_bottom = 0;
	}

} else {
	if (inputs[INPUT2].value > halfer) {
		out_bottom = inputs[INPUT2].value;
	} else {
	out_bottom = 0;
	}
}

outputs[OUTPUT1].value = out_top + out_bottom; //this should be out_top + out_top2;
lights[BLINK_LIGHT].value = inputs[INPUT1].value/10; //since oscillators are usually dealing w/ 10 volts

}

struct HalfWaveWidget : ModuleWidget {
	HalfWaveWidget(HalfWave *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/HalfWave.svg")));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<CKSS>(Vec(40, 320), module, HalfWave::INVERT, 0.0f, 1.0f, 1.0f));
		//addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 150), module, HalfWave::PITCH_PARAM, -3.0, 3.0, 0.0));
		addInput(Port::create<PJ301MPort>(Vec(33, 30), Port::INPUT, module, HalfWave::INPUT1));
		addInput(Port::create<PJ301MPort>(Vec(33, 90), Port::INPUT, module, HalfWave::INPUT2));
		addInput(Port::create<PJ301MPort>(Vec(60, 320), Port::INPUT, module, HalfWave::INVERT_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(33, 250), Port::OUTPUT, module, HalfWave::OUTPUT1));
		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec((box.size.x-10)/2,200), module, HalfWave::BLINK_LIGHT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHalfWave = Model::create<HalfWave, HalfWaveWidget>("AmateurResearch", "HalfWave", "HalfWave", EFFECT_TAG);
