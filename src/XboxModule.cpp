#include "plugin.hpp"
#include <windows.h>
#include <xinput.h>

struct XboxModule : Module {
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	
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
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	}

	void process(const ProcessArgs& args) override {
		if (XInputGetState(0, &state) == ERROR_SUCCESS) {
			bool A_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
			bool B_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
			bool X_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
			bool Y_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
			bool left_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
			bool right_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
			bool up_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
			bool down_button_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
			bool left_shoulder_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
			bool right_shoulder_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
			bool left_analog_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
			bool right_analog_pressed = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
			outputs[A_BUTTON_OUTPUT].setVoltage(A_button_pressed ? 5.f : .0);
			outputs[B_BUTTON_OUTPUT].setVoltage(B_button_pressed ? 5.f : .0);
			outputs[X_BUTTON_OUTPUT].setVoltage(X_button_pressed ? 5.f : .0);
			outputs[Y_BUTTON_OUTPUT].setVoltage(Y_button_pressed ? 5.f : .0);
			outputs[DIGITAL_LEFT_OUTPUT].setVoltage(left_button_pressed ? 5.f : .0);
			outputs[DIGITAL_RIGHT_OUTPUT].setVoltage(right_button_pressed ? 5.f : .0);
			outputs[DIGITAL_UP_OUTPUT].setVoltage(up_button_pressed ? 5.f : .0);
			outputs[DIGITAL_DOWN_OUTPUT].setVoltage(down_button_pressed ? 5.f : .0);
			outputs[LEFT_SHOULDER_OUTPUT].setVoltage(left_shoulder_pressed ? 5.f : .0);
			outputs[RIGHT_SHOULDER_OUTPUT].setVoltage(right_shoulder_pressed ? 5.f : .0);
			outputs[LEFT_ANALOG_BUTTON_OUTPUT].setVoltage(left_analog_pressed ? 5.f : .0);
			outputs[RIGHT_ANALOG_BUTTON_OUTPUT].setVoltage(right_analog_pressed ? 5.f : .0);

			float normLX = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
			float normLY = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);
			float normRX = fmaxf(-1, (float) state.Gamepad.sThumbRX / 32767);
			float normRY = fmaxf(-1, (float) state.Gamepad.sThumbRY / 32767);
			outputs[LEFT_ANALOG_X_OUTPUT].setVoltage(normLX * 5.f);
			outputs[LEFT_ANALOG_Y_OUTPUT].setVoltage(normLY * 5.f);
			outputs[RIGHT_ANALOG_X_OUTPUT].setVoltage(normRX * 5.f);
			outputs[RIGHT_ANALOG_Y_OUTPUT].setVoltage(normRY * 5.f);

			float leftTrigger = (float) state.Gamepad.bLeftTrigger / 255;
			float rightTrigger = (float) state.Gamepad.bRightTrigger / 255;
			outputs[LEFT_TRIGGER_OUTPUT].setVoltage(leftTrigger * 5.f);
			outputs[RIGHT_TRIGGER_OUTPUT].setVoltage(rightTrigger * 5.f);

			WORD leftVibration = abs(inputs[RUMBLE_LEFT_INPUT].getVoltage() / 5.f) * 65535;
			WORD rightVibration = abs(inputs[RUMBLE_RIGHT_INPUT].getVoltage() / 5.f) * 65535;
			vibration.wLeftMotorSpeed = leftVibration;
			vibration.wRightMotorSpeed = rightVibration;
			XInputSetState(0, &vibration);
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