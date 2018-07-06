#include "AmateurResearch.hpp"
#include <iostream>

struct Sign : Module {
	enum ParamIds {
		PITCH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};


	float phase = 0.0;
	float blinkPhase = 0.0;



	Sign() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

/** Advances the module by 1 audio frame with duration 1.0 / gSampleRate
Override this method to read inputs and params, and to write outputs and lights.
*/
void Sign::step() {
//so this step function basically does what (1:fs) / 1 did in Octave. Or my for loop did in the sine_how2.cpp

	// Implement a simple sine oscillator
	float deltaTime = engineGetSampleTime();

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].value;
	pitch += inputs[PITCH_INPUT].value; //TEMP OFF
	pitch = clamp(pitch, -4.0f, 4.0f);  //controls the input ranges, somehow
	// The default pitch is C4
	float freq = 261.626f * powf(2.0f, pitch); //the powf part has something do with the CV control.
	//261.626 sets middle frequency. multiplying that will shift the range, so it'll go higher

	// Accumulate the phase
	//phase += freq * deltaTime;
	phase = phase + freq * deltaTime; //sample time is 1/sampleRate so 0.00002267573696. so deltatime is the sampleTime really

//reformatted this to make more sense to me
//without this it the sine gets all stair-steppy... but not sure how this works.
if (phase >= 1.0f) {   //if phase is more or equal to 1
		//phase = phase - 1.0f;
		phase = phase - 1.0f;  /// with a value of 2, it sounds the same. with 0.5, i only get half the waveform. with zero. stairsteppy.
		// have the -1.0f value be CV controlled?
}


	// Compute the sine output
	float sine = sinf(2.0f * M_PI * phase);
	//float sine = sinf(2.0f * M_PI * (phase + freq * deltaTime)); //this kind of works

	outputs[SINE_OUTPUT].value = 5.0f * sine; //without the 5.0f *, we would have 2Vpp instead of 10Vpp







	// Blink light at 1Hz
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0f)
		blinkPhase -= 1.0f;
	lights[BLINK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
}


struct SignWidget : ModuleWidget {
	SignWidget(Sign *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Sign.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 87), module, Sign::PITCH_PARAM, -3.0, 3.0, 0.0));
		addInput(Port::create<PJ301MPort>(Vec(33, 186), Port::INPUT, module, Sign::PITCH_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(33, 275), Port::OUTPUT, module, Sign::SINE_OUTPUT));
		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, Sign::BLINK_LIGHT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelSign = Model::create<Sign, SignWidget>("AmateurResearch", "Sign", "Sign", OSCILLATOR_TAG);
