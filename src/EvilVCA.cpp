#include "AmateurResearch.hpp"
//#include "dsp/decimator.hpp"
#include <chrono>
#include <random>
#include <iostream>
#include "dsp/digital.hpp"



// just setting up parameters and inputs and outputs
struct EvilVCA : Module {
	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		BOOST_PARAM,
		BOOST_CLIPTYPE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		EXP1_INPUT,
		LIN1_INPUT,
		IN1_INPUT,
		EXP2_INPUT,
		LIN2_INPUT,
		IN2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
EvilVCA() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
void step() override; //don't know what this is
};


//noise stuff copy and paste!
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);
std::normal_distribution<double> distribution (0.0,0.002);


//the real code that makes it work
//i think input, param and out actual types... like int, float, etc

//the amperstand is a reference parameter. this explains it pretty well. the names &in &level, etc are actually DEFINED here.
//https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/functions/refparams.html

static void stepChannel(Input &in, Param &level, Param &boost, Param &boostcliptype, Input &lin, Input &exp, Output &out) {

const float PI = 3.1415927;
float v = in.value;
//boost then clamp the value to provide a bit of distortion and noise


//this noise make the power go up quite a bit, figure out a replacement
float noise = distribution(generator)*(boost.value-1); // make noise go up as boost knob goes up.


//float noise = 0.0;
//hardclip   (swith is UP)
if (boostcliptype.value == 1){
	v = (in.value * level.value)*boost.value+noise;
	v = clamp (v, -10.0f, 10.0f); //clamping values after boosting makes it clip to square waves when turned up

} else {

//softclip (swith is DOWN)   //https://www.kvraudio.com/forum/viewtopic.php?t=195315
	v = (in.value * level.value)*boost.value;
	float vtan = atan(v) * (2*PI); //could also use tanh instead
	float cboost = rescale (boost.value,1.0f,5.0f,0.0f,1.0f); //rescale the boost value (from its default) so it works with the crossfade
	v = crossfade(v,vtan,cboost)+noise; //crossfade between normal and softclipped version.
	v = clamp (v, -10.0f, 10.0f); //clamping values after boosting makes it clip to square waves when turned up
}



	//simple volume control. if level value is 1 (all the way up), then 1*1 = 1. If it's 0.5 then 1 & 0.5 = 0.5, so half the volume.

	if (lin.active)

		//v *= clamp(lin.value / 10.0f, 0.0f, 1.0f);
		//rewritten from above to be more clear.

		//this attenuates by the Linear IN CV...
		//it's divided by 10, why? because the CV inputs are on -10v to 10v scale?

		v = v * clamp(lin.value / 10.0f, 0.0f, 1.0f);
//this clamps v between 0 and 1.
// if lin.value is less than 0.0f, return 0.0f.
// if high is less than lin.value, return 1.0f
// otherwise return lin.value
//clamp reduces the linear input down to to -10 to 10 if it's hotter (or in the code's case, 0 to 1) ... . for example if i boost a signal before sending it in.


	const float expBase = 50.0f;
	if (exp.active)
	//this gives an exponetial response
		v *= rescale(powf(expBase, clamp(exp.value / 10.0f, 0.0f, 1.0f)), 1.0f, expBase, 0.0f, 1.0f);
		out.value = v;
	}

void EvilVCA::step() {
	stepChannel(inputs[IN1_INPUT], params[LEVEL1_PARAM], params[BOOST_PARAM], params[BOOST_CLIPTYPE_PARAM], inputs[LIN1_INPUT], inputs[EXP1_INPUT], outputs[OUT1_OUTPUT]);
	stepChannel(inputs[IN2_INPUT], params[LEVEL2_PARAM], params[BOOST_PARAM], params[BOOST_CLIPTYPE_PARAM], inputs[LIN2_INPUT], inputs[EXP2_INPUT], outputs[OUT2_OUTPUT]);
}


//below is all UI stuff
struct EvilVCAWidget : ModuleWidget {
	EvilVCAWidget(EvilVCA *module);
};



EvilVCAWidget::EvilVCAWidget(EvilVCA *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/EvilVCA.svg")));



	addChild(Widget::create<ScrewSilver>(Vec(15, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x-30, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(15, 365)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x-30, 365)));

//The value ranges at the end are minValue, maxValue and defaultValue.
	addParam(ParamWidget::create<RoundLargeBlackKnob>(Vec(27, 57), module, EvilVCA::LEVEL1_PARAM, 0.0f, 1.0f, 0.5f));
	addParam(ParamWidget::create<RoundLargeBlackKnob>(Vec(27, 222), module, EvilVCA::LEVEL2_PARAM, 0.0f, 1.0f, 0.5f));
	addParam(ParamWidget::create<RoundSmallBlackKnob>(Vec(27, 27), module, EvilVCA::BOOST_PARAM, 1.0f, 5.0f, 1.0f));
	addParam(ParamWidget::create<CKSS>(Vec(60, 32), module, EvilVCA::BOOST_CLIPTYPE_PARAM, 0.0f, 1.0f, 1.0f));
	addInput(Port::create<PJ301MPort>(Vec(11, 113), Port::INPUT, module, EvilVCA::EXP1_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(54, 113), Port::INPUT, module, EvilVCA::LIN1_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(11, 156), Port::INPUT, module, EvilVCA::IN1_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(11, 276), Port::INPUT, module, EvilVCA::EXP2_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(54, 276), Port::INPUT, module, EvilVCA::LIN2_INPUT));
	addInput(Port::create<PJ301MPort>(Vec(11, 320), Port::INPUT, module, EvilVCA::IN2_INPUT));



	addOutput(Port::create<PJ301MPort>(Vec(54, 156), Port::OUTPUT, module, EvilVCA::OUT1_OUTPUT));
	addOutput(Port::create<PJ301MPort>(Vec(54, 320), Port::OUTPUT, module, EvilVCA::OUT2_OUTPUT));

}
//module info like name and tag
Model *modelEvilVCA = Model::create<EvilVCA, EvilVCAWidget>("AmateurResearch", "Evil VCA", "Evil VCA", AMPLIFIER_TAG, DUAL_TAG);
