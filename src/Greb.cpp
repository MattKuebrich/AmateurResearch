// Sine Wave Gen

#include "AmateurResearch.hpp"
#include <iostream>

struct Greb : Module {
	enum ParamIds {
		PITCH_PARAM, CUTOFF_PARAM, RESONANCE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT, TRIGGER_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT, SQUARE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};


	float phase = 0.0;
	float trig = 0.0;
	float freq = 100; //hz



	// By Paul Kellett
	// http://www.musicdsp.org/showone.php?id=29
// http://www.martin-finke.de/blog/articles/audio-plugins-013-filter/
double buf0, buf1, buf2, buf3 = 0.0;
double cutoff = 1.0;

	double LPFilter (double inputValue, double cutoff, double feedbackAmount) {

	    //buf0 += cutoff * (inputValue - buf0);
			buf0 += cutoff * (inputValue - buf0 + feedbackAmount * (buf0 - buf1)); // resonance added
			buf1 += cutoff * (buf0 - buf1);


	            return buf1;  // LP
							//return inputValue - buf0; // HP
							//return buf0 - buf1 ; // BP
	}


	Greb() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

/** Advances the module by 1 audio frame with duration 1.0 / engineGetSampleRate()
Override this method to read inputs and params, and to write outputs and lights.
*/
void Greb::step() {

	//std::string phaseout = std::to_string(phase);
  //debug("error: %d", errno);

//so this step function basically does what (1:fs) / 1 did in Octave. Or my for loop did in the sine_how2.cpp

	// Compute the frequency from the pitch parameter and input

//scale the -3 to 3 from the knob and translate it into 65hz to 2000hz (a typical vco freq range)
	float pitch = rescale(params[PITCH_PARAM].value,-3.0f,3.0f,65.0f,2000.0f);

//inputs[PITCH_INPUT].value;
//pitch += inputs[PITCH_INPUT].value; //TEMP OFF
//float sine = sinf(2.0f * M_PI * (phase + freq * deltaTime)); //this kind of works





	float sampleTime = engineGetSampleTime();
//	float sampleRate = engineGetSampleRate();

	phase = phase + (pitch * sampleTime);
	//phase = phase + sampleTime;


//this basically makes phase go from smoothly go from 0 to 1 to 0 to 1, and so on... otherwise it the osc gets all stairsteppy
		if (phase >= 1.0f) {   //if phase is more or equal to 1
				//phase = phase - 1.0f;
				phase = phase - 1.0f;  /// once phase = 1, this subtracts 1, bringing phase back to zero
				// have the - 1.0f value be CV controlled?
		}
		//printf("%f\n",phase);


	float sine = sin(2*M_PI*phase);
	double square = 4*(sin(2*M_PI*phase)+(sin(6*M_PI*phase))/3+(sin(10*M_PI*phase))/5+(sin(14*M_PI*phase))/7)/5; //square wave approximation


if (inputs[TRIGGER_INPUT].value >= 1.0f) {
	trig = 1;
}  else {
	trig = 1; // should be 0
}

square = LPFilter(square,params[CUTOFF_PARAM].value,params[RESONANCE_PARAM].value);

	outputs[SINE_OUTPUT].value = (sine*5.0f)  * trig  ; //boost voltage
	outputs[SQUARE_OUTPUT].value = (square*5.0f) * trig ; //boost voltage




	//outputs[SQUARE_OUTPUT].value = (square*5.0f) * trig ; //boost voltage
}







struct GrebWidget : ModuleWidget {
	GrebWidget(Greb *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Greb.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 90), module, Greb::PITCH_PARAM, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 130), module, Greb::CUTOFF_PARAM, 0, 0.1, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 170), module, Greb::RESONANCE_PARAM, 0, 2, 0.0));
		//addInput(Port::create<PJ301MPort>(Vec(33, 186), Port::INPUT, module, Greb::PITCH_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(33, 230.5), Port::INPUT, module, Greb::TRIGGER_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(33, 275), Port::OUTPUT, module, Greb::SINE_OUTPUT));
		addOutput(Port::create<PJ301MPort>(Vec(33, 325), Port::OUTPUT, module, Greb::SQUARE_OUTPUT));

		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, Greb::BLINK_LIGHT));

	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelGreb = Model::create<Greb, GrebWidget>("AmateurResearch", "Greb", "Greb", OSCILLATOR_TAG);
