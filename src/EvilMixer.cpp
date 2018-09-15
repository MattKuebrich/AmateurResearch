#include "AmateurResearch.hpp"


struct EvilMixer : Module {
	enum ParamIds {
		CH1_PARAM,
		CH2_PARAM,
		CH3_PARAM,
		CH4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		NUM_OUTPUTS
	};


	EvilMixer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};


void EvilMixer::step() {
	float in1 = inputs[IN1_INPUT].value * params[CH1_PARAM].value;
	float in2 = inputs[IN2_INPUT].value * params[CH2_PARAM].value;
	float in3 = inputs[IN3_INPUT].value * params[CH3_PARAM].value;
	float in4 = inputs[IN4_INPUT].value * params[CH4_PARAM].value;

	float out = in1 + in2 + in3 + in4;



outputs[OUT1_OUTPUT].value = out;


}


struct EvilMixerWidget : ModuleWidget {

	int offset = 25;
	int knoboffset = 4;
	EvilMixerWidget(EvilMixer *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/EvilMixer.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(15, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(15, 365)));

		addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 32+offset-knoboffset), module, EvilMixer::CH1_PARAM, 0.0, 1.0, 0.0));
		addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 85+offset-knoboffset), module, EvilMixer::CH2_PARAM, 0.0, 1.0, 0.0));
		addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 137+offset-knoboffset), module, EvilMixer::CH3_PARAM, 0.0, 1.0, 0.0));
		addParam(ParamWidget::create<MixerKnobWhite>(Vec(47, 190+offset-knoboffset), module, EvilMixer::CH4_PARAM, 0.0, 1.0, 0.0));

		addInput(Port::create<PJ301MPort>(Vec(13, 32+offset), Port::INPUT, module, EvilMixer::IN1_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(13, 85+offset), Port::INPUT, module, EvilMixer::IN2_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(13, 137+offset), Port::INPUT, module, EvilMixer::IN3_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(13, 190+offset), Port::INPUT, module, EvilMixer::IN4_INPUT));
		addOutput(Port::create<PJ301MPort>(Vec(32.7, 300), Port::OUTPUT, module, EvilMixer::OUT1_OUTPUT));
	}
};


Model *modelEvilMixer = Model::create<EvilMixer, EvilMixerWidget>("AmateurResearch", "Evil Mixer", "Evil Mixer", MIXER_TAG);
