#include "plugin.hpp"
#include <windows.h>
#include "Gamepad.hpp"

struct XboxModule : Module {
	GamepadState gamepadState;
	Gamepad gamepad;
	float leftVibration;
	float rightVibration;
	
	enum ProcessingState {
		READ_CONTROLLER,
		APPLY_VALUES,
		SET_VIBRATION
	} currentState = READ_CONTROLLER;
	
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		RUMBLE_LEFT_INPUT,
		RUMBLE_RIGHT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		RIGHT_SHOULDER_OUTPUT,
		LEFT_TRIGGER_OUTPUT,
		RIGHT_TRIGGER_OUTPUT,
		LEFT_SHOULDER_OUTPUT,
		Y_BUTTON_OUTPUT,
		LEFT_ANALOG_BUTTON_OUTPUT,
		LEFT_ANALOG_X_OUTPUT,
		B_BUTTON_OUTPUT,
		X_BUTTON_OUTPUT,
		LEFT_ANALOG_Y_OUTPUT,
		A_BUTTON_OUTPUT,
		DIGITAL_UP_OUTPUT,
		RIGHT_ANALOG_BUTTON_OUTPUT,
		DIGITAL_LEFT_OUTPUT,
		DIGITAL_RIGHT_OUTPUT,
		RIGHT_ANALOG_X_OUTPUT,
		DIGITAL_DOWN_OUTPUT,
		RIGHT_ANALOG_Y_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	XboxModule() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}
	
	bool readGamepadState() {
		return gamepad.readGamepadState(&gamepadState);
	}
	
	void applyVoltages() {
		outputs[A_BUTTON_OUTPUT].setVoltage(gamepadState.aButtonPressed ? 5.f : .0);
		outputs[B_BUTTON_OUTPUT].setVoltage(gamepadState.bButtonPressed ? 5.f : .0);
		outputs[X_BUTTON_OUTPUT].setVoltage(gamepadState.xButtonPressed ? 5.f : .0);
		outputs[Y_BUTTON_OUTPUT].setVoltage(gamepadState.yButtonPressed ? 5.f : .0);
		outputs[DIGITAL_LEFT_OUTPUT].setVoltage(gamepadState.leftPressed ? 5.f : .0);
		outputs[DIGITAL_RIGHT_OUTPUT].setVoltage(gamepadState.rightPressed ? 5.f : .0);
		outputs[DIGITAL_UP_OUTPUT].setVoltage(gamepadState.upPressed ? 5.f : .0);
		outputs[DIGITAL_DOWN_OUTPUT].setVoltage(gamepadState.downPressed ? 5.f : .0);
		outputs[LEFT_SHOULDER_OUTPUT].setVoltage(gamepadState.leftShoulderButtonPressed ? 5.f : .0);
		outputs[RIGHT_SHOULDER_OUTPUT].setVoltage(gamepadState.rightShoulderButtonPressed ? 5.f : .0);
		outputs[LEFT_ANALOG_BUTTON_OUTPUT].setVoltage(gamepadState.leftThumbPressed ? 5.f : .0);
		outputs[RIGHT_ANALOG_BUTTON_OUTPUT].setVoltage(gamepadState.rightThumbPressed ? 5.f : .0);

		outputs[LEFT_ANALOG_X_OUTPUT].setVoltage(gamepadState.leftThumbX * 5.f);
		outputs[LEFT_ANALOG_Y_OUTPUT].setVoltage(gamepadState.leftThumbY * 5.f);
		outputs[RIGHT_ANALOG_X_OUTPUT].setVoltage(gamepadState.rightThumbX * 5.f);
		outputs[RIGHT_ANALOG_Y_OUTPUT].setVoltage(gamepadState.rightThumbY * 5.f);

		outputs[LEFT_TRIGGER_OUTPUT].setVoltage(gamepadState.leftTrigger * 5.f);
		outputs[RIGHT_TRIGGER_OUTPUT].setVoltage(gamepadState.rightTrigger * 5.f);

		leftVibration = abs(inputs[RUMBLE_LEFT_INPUT].getVoltage() / 5.f);
		rightVibration = abs(inputs[RUMBLE_RIGHT_INPUT].getVoltage() / 5.f);
	}
	
	void setVibration() {
		gamepad.setVibration(leftVibration, rightVibration);
	}

	void process(const ProcessArgs& args) override {
		switch (currentState) {
			case READ_CONTROLLER:
				if (readGamepadState())
					currentState = APPLY_VALUES;
				break;
			case APPLY_VALUES:
				applyVoltages();
				currentState = SET_VIBRATION;
				break;
			case SET_VIBRATION:
				setVibration();
				currentState = READ_CONTROLLER;
				break;
		}
	}
};


struct XboxModuleWidget : ModuleWidget {
	XboxModuleWidget(XboxModule* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/XboxModule.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(73.232, 105.304)), module, XboxModule::RUMBLE_LEFT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(179.071, 105.304)), module, XboxModule::RUMBLE_RIGHT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(145.662, 32.072)), module, XboxModule::RIGHT_SHOULDER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(89.803, 32.34)), module, XboxModule::LEFT_TRIGGER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(162.5, 32.34)), module, XboxModule::RIGHT_TRIGGER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(105.839, 32.607)), module, XboxModule::LEFT_SHOULDER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(162.767, 46.505)), module, XboxModule::Y_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(84.992, 50.247)), module, XboxModule::LEFT_ANALOG_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(96.351, 51.048)), module, XboxModule::LEFT_ANALOG_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(171.053, 54.79)), module, XboxModule::B_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(152.878, 55.325)), module, XboxModule::X_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.595, 60.937)), module, XboxModule::LEFT_ANALOG_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(163.034, 63.076)), module, XboxModule::A_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(105.571, 66.283)), module, XboxModule::DIGITAL_UP_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(144.058, 74.835)), module, XboxModule::RIGHT_ANALOG_BUTTON_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(95.95, 75.637)), module, XboxModule::DIGITAL_LEFT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(114.659, 76.172)), module, XboxModule::DIGITAL_RIGHT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(154.749, 76.172)), module, XboxModule::RIGHT_ANALOG_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(105.571, 85.259)), module, XboxModule::DIGITAL_DOWN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(144.86, 86.328)), module, XboxModule::RIGHT_ANALOG_Y_OUTPUT));
	}
};


Model* modelXboxModule = createModel<XboxModule, XboxModuleWidget>("XboxModule");