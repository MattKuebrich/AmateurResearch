// Sine Wave Gen

#include "AmateurResearch.hpp"
#include <iostream>

struct Sign2 : Module {
	enum ParamIds {
		PITCH_PARAM,SLOPE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		GATE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT, SQUARE_OUTPUT, TRIANGLE_OUTPUT, SAW_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};


	float phase = 0.0;
	float phase2 = 0.0;
	float phase3 = 0.0;
	float squareout  = 0.0;
	float triangleout  = 0.0;
	float blinkPhase = 0.0;
	float fixedfreq = 440.0f; //hz


	Sign2() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

/** Advances the module by 1 audio frame with duration 1.0 / engineGetSampleRate()
Override this method to read inputs and params, and to write outputs and lights.
*/
void Sign2::step() {

	//std::string phaseout = std::to_string(phase);
  //debug("error: %d", errno);

//so this step function basically does what (1:fs) / 1 did in Octave. Or my for loop did in the sine_how2.cpp

	// Compute the frequency from the pitch parameter and input

//scale the -3 to 3 from the knob and translate it into 65hz to 2000hz (a typical vco freq range)
	float pitch = rescale(params[PITCH_PARAM].value,-3.0f,3.0f,65.0f,2000.0f);
float slope = params[SLOPE_PARAM].value;
//inputs[PITCH_INPUT].value;
//pitch += inputs[PITCH_INPUT].value; //TEMP OFF
//float sine = sinf(2.0f * M_PI * (phase + freq * deltaTime)); //this kind of works





	//float sampleTime = engineGetSampleTime(); //1/samplerate
	float sampleTime = 1/engineGetSampleRate(); //same thing
//	float sampleRate = engineGetSampleRate();

	phase = phase + (pitch * sampleTime); // "phase accumulator" is needed
	phase2 = phase2 + (pitch * sampleTime); // "phase accumulator" is needed

	//phase2 = phase2 + (pitch * sampleTime); // "phase accumulator" is needed

//	phase = phase + (fixedfreq * sampleTime); // "phase accumulator" is needed


//phase = phase* sampleTime;
// so this is 0 + (440 * 1/44100), so numbers are small..below zero



//printf("%f\n",sampleTime);
//printf("%f\n",phase);
	//phase = phase + sampleTime;


//this basically makes phase go from smoothly go from 0 to 1 to 0 to 1, and so on... otherwise it the osc gets all stairsteppy
// this generates a sawtooth wave by itself
		if (phase >= 1.0f) {   //if phase is more or equal to 1
				//phase = phase - 1.0f;
				phase = phase - 1.0f;  /// once phase = 1, this subtracts 1, bringing phase back to zero
				// have the - 1.0f value be CV controlled?
		}




//triangleout = (phase2 < 0.5) ? (-1.0 + 4.0*phase2) : (1.0 - 4.0*(phase2 - 0.5));
//taken from fundamental.. reformatted into regular if else
//dont really understand what's going on



/// goofy morphy thing
// if (phase < slope) {
// triangleout = -1.0 + slope * phase;
// } else {
// triangleout = 1.0 - slope * (phase - 0.5);
// }


// //triangle out
// if (phase < 0.5) {
// triangleout = -1.0 + 4.0 * phase;
// } else {
// triangleout = 1.0 - 4.0 * (phase - 0.5);
// }


//phase is going from 0 to 1, then back to 0.
//triangle out alt way
// if (phase < 0.25) {
// triangleout = 4.0 * phase;
// } else if (phase < 0.75) {
// triangleout = 1.0 - 4.0 * (phase - 0.25);
// } else {
// triangleout = 4.0 * (phase - 0.75) - 1.0;
// }


if (inputs[GATE_INPUT].value){

if (phase > 0.5) {
triangleout = 0;
} else {
triangleout = 1;
}

}



		//printf("%f\n",phase2);

		//square out
		if (phase < 0.5){
		squareout = 0;
		} else {
		squareout = 1;
		}

		//tri out
	//	if (phase < 0.5){
	//	triangleout = triangleout+ sampleTime;
//		}




	//float sine = sin(2*M_PI*phase);
	float sine = sin(2*M_PI*phase); //fixed frequency


	//float square = 4*(sin(2*M_PI*phase)+(sin(6*M_PI*phase))/3+(sin(10*M_PI*phase))/5+(sin(14*M_PI*phase))/7)/5; //square wave approximation

	//outputs[SINE_OUTPUT].value = sine*5.0f; //boost voltage from -1v to 1v to -5v to 5v
	//outputs[SINE_OUTPUT].value = sine; //boost voltage
	outputs[SINE_OUTPUT].value = sine*5.0f; //boost voltage from -1v to 1v to -5v to 5v
	outputs[SQUARE_OUTPUT].value = (squareout - 0.5f)*10.0f; //apply offset to goes around 0v.... boost voltage to +-5v
//	outputs[TRIANGLE_OUTPUT].value = triangleout*10.0f; //apply offset to goes around 0v.... boost voltage to +-5v
	outputs[TRIANGLE_OUTPUT].value = triangleout*5.0f; //apply offset to goes around 0v.... boost voltage to +-5v
	outputs[SAW_OUTPUT].value = (phase - 0.5f)*10.0f; //apply offset to goes around 0v.... boost voltage to +-5v



}







struct Sign2Widget : ModuleWidget {
	Sign2Widget(Sign2 *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Sign2.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(27.1, 48.3), module, Sign2::PITCH_PARAM, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(27.1, 131.3), module, Sign2::SLOPE_PARAM, 0, 4.0, 0.0));
		addInput(Port::create<PJ301MPort>(Vec(32.6, 207.1), Port::INPUT, module, Sign2::GATE_INPUT));

		addOutput(Port::create<PJ301MPort>(Vec(12.8, 261.8), Port::OUTPUT, module, Sign2::SINE_OUTPUT));
		addOutput(Port::create<PJ301MPort>(Vec(51.3, 261.8), Port::OUTPUT, module, Sign2::SQUARE_OUTPUT));
		addOutput(Port::create<PJ301MPort>(Vec(12.8 , 319.7), Port::OUTPUT, module, Sign2::TRIANGLE_OUTPUT));
		addOutput(Port::create<PJ301MPort>(Vec(51.3, 319.7), Port::OUTPUT, module, Sign2::SAW_OUTPUT));

		//addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, Sign2::BLINK_LIGHT));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelSign2 = Model::create<Sign2, Sign2Widget>("AmateurResearch", "Sign2", "Sign2", OSCILLATOR_TAG);
