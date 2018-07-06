//right now this takes 2 input signals and takes the top half from one and the bottom half from the other.
//it works but it's not that interesting... it sounds nearly the same as if they just went through a mixer
// the switch switches the order and the jack next to it sort of modulates it (just turns it ON or OFF).

#include "AmateurResearch.hpp"


struct Rectify : Module {
	enum ParamIds {
		SWEEP_PARAM, RECT,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT1,INPUT2,INPUT3,SWEEP_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT1,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,BLINK_LIGHT2,
		NUM_LIGHTS
	};

float phase = 1.0;
float newout1 = 0;
float newout2 = 0;
float newout3 = 0;

	Rectify() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};





void Rectify::step() {
//float sine = sinf(2.0f * M_PI * phase);
	//outputs[OUTPUT1].value = 5.0f * sine;


//halfwave rectifier thing?


float sweeper = params[SWEEP_PARAM].value;

//figure out how to map this better for CV... works better if I use an attenuator on it
sweeper += inputs[SWEEP_INPUT].value;

	// if (inputs[INPUT1].value < 0.333) {
	// 	newout1 = inputs[INPUT1].value;
	// } else {
	// newout1 = 0;
	// }


		if (inputs[INPUT1].value > sweeper*-1 && inputs[INPUT1].value < sweeper) {
			newout1 = inputs[INPUT1].value;
		} else {
			newout1 = inputs[INPUT1].value*-1;
		}

	if (inputs[INPUT2].value > sweeper*-1 && inputs[INPUT2].value < sweeper) {
		newout2 = inputs[INPUT2].value;
	} else {
		newout2 = inputs[INPUT2].value*-1;
	}

	if (inputs[INPUT3].value > sweeper*-1 && inputs[INPUT3].value < sweeper) {
		newout3 = inputs[INPUT3].value;
	} else {
		newout3 = inputs[INPUT3].value*-1;
	}



	// if (inputs[INPUT3].value > 0.333) {
	// 	newout3 = inputs[INPUT3].value;
	// } else {
	// newout3 = 0;
	// }

//divided by 3 to get "normal" output voltage, not sure if that's correct
outputs[OUTPUT1].value = (newout1 + newout2 + newout3)/3;



//this does a "mult"
	//outputs[OUTPUT1].value = inputs[INPUT1].value;
	//outputs[OUTPUT2].value = inputs[INPUT1].value;


	//feeding the same thing into both will cancel them out!
	//outputs[OUTPUT1].value = newout+newout2;
	lights[BLINK_LIGHT].value = inputs[INPUT1].value/10; //since oscillators are usually dealing w/ 10 volts



}

struct RectifyWidget : ModuleWidget {
	RectifyWidget(Rectify *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Rectify.svg")));

//just the screws in the module
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

// the switch
		//addParam(ParamWidget::create<CKSS>(Vec(40, 320), module, Rectify::RECT, 0.0f, 1.0f, 1.0f));


// can minus 45px - 18 (half the width of the knob)
		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(27,142 ), module, Rectify::SWEEP_PARAM, 0, 5, 0.0));
//addInput(Port::create<PJ301MPort>(Vec(11, 276), Port::INPUT, module, VCO::PITCH_INPUT));

		addInput(Port::create<PJ301MPort>(Vec(33, 30), Port::INPUT, module, Rectify::INPUT1));
		addInput(Port::create<PJ301MPort>(Vec(33, 60), Port::INPUT, module, Rectify::INPUT2));
		addInput(Port::create<PJ301MPort>(Vec(33, 90), Port::INPUT, module, Rectify::INPUT3));

		//CV input that controls flip switch
		addInput(Port::create<PJ301MPort>(Vec(33, 210), Port::INPUT, module, Rectify::SWEEP_INPUT));

		//main output
		addOutput(Port::create<PJ301MPort>(Vec(33, 250), Port::OUTPUT, module, Rectify::OUTPUT1));
//centering LED
		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec((box.size.x-10)/2,300), module, Rectify::BLINK_LIGHT));

	//	addOutput(Port::create<PJ301MPort>(Vec(33, 200), Port::OUTPUT, module, Rectify::OUTPUT2));
	//	addOutput(Port::create<PJ301MPort>(Vec(33, 250), Port::OUTPUT, module, Rectify::OUTPUT3));
		//addChild(ModuleLightWidget::create<MediumLight<BlueLight>>(Vec(41, 59), module, Rectify::BLINK_LIGHT2));
		//addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 49), module, Rectify::BLINK_LIGHT));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelRectify = Model::create<Rectify, RectifyWidget>("AmateurResearch", "Rectify", "Rectify", EFFECT_TAG);
