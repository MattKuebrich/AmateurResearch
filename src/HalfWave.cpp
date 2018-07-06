//right now this takes 2 input signals and takes the top half from one and the bottom half from the other.
//it works but it's not that interesting... it sounds nearly the same as if they just went through a mixer
// the switch switches the order and the jack next to it sort of modulates it (just turns it ON or OFF).

#include "AmateurResearch.hpp"


struct HalfWave : Module {
	enum ParamIds {
		PITCH_PARAM, RECT,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT1,INPUT2,INPUT3,
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
float newout = 0;
float newout2 = 0;
float newout_rev = 0;
float newout2_rev = 0;
	HalfWave() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};





void HalfWave::step() {
//float sine = sinf(2.0f * M_PI * phase);
	//outputs[OUTPUT1].value = 5.0f * sine;


//halfwave rectifier thing?




if (params[RECT].value == 0){
	if (inputs[INPUT1].value < 0.5) {
		newout = inputs[INPUT1].value;
	} else {
	newout = 0;
	}

} else {
	if (inputs[INPUT1].value > 0.5) {
		newout = inputs[INPUT1].value;
	} else {
	newout = 0;
	}
}


if (params[RECT].value == 1){
	if (inputs[INPUT2].value < 0.5) {
		newout2 = inputs[INPUT2].value;
	} else {
	newout2 = 0;
	}

} else {
	if (inputs[INPUT2].value > 0.5) {
		newout2 = inputs[INPUT2].value;
	} else {
	newout2 = 0;
	}
}

if (inputs[INPUT3].value < 0.5) {
newout = newout *-1;
newout2 = newout2 *-1;
}



outputs[OUTPUT1].value = newout + newout2;



//this does a "mult"
	//outputs[OUTPUT1].value = inputs[INPUT1].value;
	//outputs[OUTPUT2].value = inputs[INPUT1].value;


	//feeding the same thing into both will cancel them out!
	//outputs[OUTPUT1].value = newout+newout2;
	lights[BLINK_LIGHT].value = inputs[INPUT1].value/10; //since oscillators are usually dealing w/ 10 volts



}

struct HalfWaveWidget : ModuleWidget {
	HalfWaveWidget(HalfWave *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/HalfWave.svg")));

//just the screws in the module
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

// the switch
		addParam(ParamWidget::create<CKSS>(Vec(40, 320), module, HalfWave::RECT, 0.0f, 1.0f, 1.0f));

		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 150), module, HalfWave::PITCH_PARAM, -3.0, 3.0, 0.0));
//addInput(Port::create<PJ301MPort>(Vec(11, 276), Port::INPUT, module, VCO::PITCH_INPUT));

		addInput(Port::create<PJ301MPort>(Vec(33, 30), Port::INPUT, module, HalfWave::INPUT1));
		addInput(Port::create<PJ301MPort>(Vec(33, 90), Port::INPUT, module, HalfWave::INPUT2));
				addInput(Port::create<PJ301MPort>(Vec(60, 320), Port::INPUT, module, HalfWave::INPUT3));
		addOutput(Port::create<PJ301MPort>(Vec(33, 250), Port::OUTPUT, module, HalfWave::OUTPUT1));
//centering LED
		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec((box.size.x-10)/2,200), module, HalfWave::BLINK_LIGHT));

	//	addOutput(Port::create<PJ301MPort>(Vec(33, 200), Port::OUTPUT, module, HalfWave::OUTPUT2));
	//	addOutput(Port::create<PJ301MPort>(Vec(33, 250), Port::OUTPUT, module, HalfWave::OUTPUT3));
		//addChild(ModuleLightWidget::create<MediumLight<BlueLight>>(Vec(41, 59), module, HalfWave::BLINK_LIGHT2));
		//addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 49), module, HalfWave::BLINK_LIGHT));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHalfWave = Model::create<HalfWave, HalfWaveWidget>("AmateurResearch", "HalfWave", "HalfWave", EFFECT_TAG);
