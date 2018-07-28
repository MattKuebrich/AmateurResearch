// Experimenting with type

#include "AmateurResearch.hpp"
#include <iostream>
#include "dsp/digital.hpp"


int randomnum = 0;
float crush = 0;



//testing label
struct CenteredLabel : Widget {
	std::string text;
	int fontSize;
	CenteredLabel(int _fontSize = 12) {
		fontSize = _fontSize;
		box.size.y = BND_WIDGET_HEIGHT;
	}
	void draw(NVGcontext *vg) override {
		nvgTextAlign(vg, NVG_ALIGN_CENTER);
		nvgFillColor(vg, nvgRGB(255, 255, 255));
		nvgFontSize(vg, fontSize);
		nvgText(vg, box.pos.x, box.pos.y, text.c_str(), NULL);
	}
};


//this is actually a gradient not text...
struct TextTest : VirtualWidget {


TextTest(int x, int y) {
box.pos = Vec(x, y);
box.size = Vec(100,120); //dont really need
}

void draw(NVGcontext *vg) override {

// nvgBeginPath(vg);
// nvgRect(vg, 0, 0, box.size.x, box.size.y);
// nvgFillColor(vg, nvgRGBA(255,100,0,255));
// nvgFill(vg);

int height = 100;
NVGpaint bg = nvgRadialGradient(vg, 0,0,0, height,nvgRGBA(10,212,255,255), nvgRGBA(231,28,186,255));
//NVGpaint bg = nvgLinearGradient(vg, 0,0,0, height,nvgRGBA(10,212,255,255), nvgRGBA(231,28,186,255));

nvgFillPaint(vg, bg);
nvgFill(vg);
}

void onKey(EventKey &e) override{
printf("%s\n","keydown");
}
void onHoverKey(EventHoverKey &e) override{
printf("%s\n","hoverkey");
}
};



struct Graffiti : Module {
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
float newout1 = 0;
float newout2 = 0;
float newout3 = 0;
int shifted = 0;
int shifted2 = 0;
	Graffiti() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};



//
//can use these other bitwise operators to crush the signal
//char bitw [5] = { &, |, ^, <<, >> };

void Graffiti::step() {

	crush = params[BITCRUSH_AMT].value;

}



struct GraffitiWidget : ModuleWidget {
	GraffitiWidget(Graffiti *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Graffiti.svg")));
		//test textbox
				TextTest *tt = new TextTest(50,100);
				addChild(tt);
//just the screws in the module
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(36.683,201.583), module, Graffiti::BITCRUSH_AMT, 0, 5, 0.0));
		addOutput(Port::create<PJ301MPort>(Vec(54, 156), Port::OUTPUT, module, Graffiti::OUTPUT1));



		// test label
		CenteredLabel* const titleLabel = new CenteredLabel(10); // number is size
		CenteredLabel* const titleLabel2 = new CenteredLabel(10); // number is size

		titleLabel->box.pos = Vec(20, 15);
		titleLabel2->box.pos = Vec(20, 20);
		//	titleLabel->text = "hello!";


		float samplerate = engineGetSampleRate();
		float sampletime = engineGetSampleTime();      	/** Returns the inverse of the current sample rate */
		titleLabel->text = std::to_string(samplerate);
		titleLabel2->text = std::to_string(sampletime);

		//was trying to link knob to text, didn't work.
		//titleLabel->text = std::to_string(crush);
		//this doesn't work
		addChild(titleLabel);
		addChild(titleLabel2);


	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelGraffiti = Model::create<Graffiti, GraffitiWidget>("AmateurResearch", "Graffiti", "Graffiti", EFFECT_TAG);
