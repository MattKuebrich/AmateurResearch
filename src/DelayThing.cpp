#include "AmateurResearch.hpp"


struct DelayThing : Module {
	enum ParamIds {
		CH1_PARAM,
		DELAY_PARAM,
		CH3_PARAM,
		CH4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		TRIG_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		NUM_OUTPUTS
	};


	DelayThing() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};


float sampleRate = engineGetSampleRate();
float sampleTime = engineGetSampleTime();
//int buf [44100];
double buf [44100*4];
double buf2 [44100*4];
double buf3 [44100*4];
int i = 0;
int i2 = 0;
int i3 = 0;
int b = 0;



void DelayThing::step() {
	float in1 = inputs[IN1_INPUT].value;
//	float in1 = inputs[IN1_INPUT].value * params[CH1_PARAM].value;

	float out = in1;

        if (i <= params[DELAY_PARAM].value) {
          buf[i] = in1;
          i++;
        } else {
          i = 0;
        }

        // another delay for echo thing
        if (i2 <= params[DELAY_PARAM].value * 2) {
          buf2[i2] = in1;
          i2++;
        } else {
          i2 = 0;
        }

        // another delay for echo thing
        if (i3 <= params[DELAY_PARAM].value * 3) {
          buf3[i3] = in1;
          i3++;
        } else {
          i3 = 0;
        }


        // mix both delays plus the original output to create echo thing
	//outputs[OUT1_OUTPUT].value = (((buf[i]* params[CH1_PARAM].value)+(buf2[i2]* params[CH1_PARAM].value/2))+out);


float echo1 = buf[i]* params[CH1_PARAM].value;
float echo2 = buf2[i2]* params[CH1_PARAM].value/2;
float echo3 = buf3[i3]* params[CH1_PARAM].value/3;


outputs[OUT1_OUTPUT].value = echo1 + echo2 + echo3 + out;



//outputs[OUT1_OUTPUT].value = (((buf[i]* params[CH1_PARAM].value)+(buf2[i2]* params[CH1_PARAM].value/2))+out);




//outputs[OUT1_OUTPUT].value = buf[i];


//         // this should wait 5 seconds, start outputting
//         if (b >= sampleRate * 5) {
//           outputs[OUT1_OUTPUT].value = out;
// }
//
// b++; //this is outside the for loop so it updates every time through step()


//while (b > 44100*10) {
//outputs[OUT1_OUTPUT].value = buf[b];
//outputs[OUT1_OUTPUT].value = out;
//b++;
//}


//outputs[OUT1_OUTPUT].value = out;


}


struct DelayThingWidget : ModuleWidget {

	int offset = 25;
	int knoboffset = 4;
	DelayThingWidget(DelayThing *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/DelayThing.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(15, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(15, 365)));

		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(47, 32+offset-knoboffset), module, DelayThing::CH1_PARAM, 0.0, 1.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(47, 95+offset-knoboffset), module, DelayThing::DELAY_PARAM, 0.0, 44100, 0.0));
		//addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 137+offset-knoboffset), module, DelayThing::CH3_PARAM, 0.0, 1.0, 0.0));
		//addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 190+offset-knoboffset), module, DelayThing::CH4_PARAM, 0.0, 1.0, 0.0));

		addInput(Port::create<PJ301MPort>(Vec(13, 32+offset), Port::INPUT, module, DelayThing::IN1_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(13, 32+offset+offset+offset), Port::INPUT, module, DelayThing::TRIG_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(32.7, 300), Port::OUTPUT, module, DelayThing::OUT1_OUTPUT));
	}
};


Model *modelDelayThing = Model::create<DelayThing, DelayThingWidget>("AmateurResearch", "DelayThing", "DelayThing", MIXER_TAG);
