#pragma once

#include <SC_Button.h>
#include "LayoutInterface.h"
#include "Hardware.h"

class LayoutPedals : public LayoutInterface {

	friend class InputManager;

	public:

		virtual void update() {
			_axe->isEffectEnabled(ID_FUZZ1)   ? _leds->on(Drive1) : _leds->dim(Drive1);
			_axe->isEffectEnabled(ID_FUZZ2)   ? _leds->on(Drive2) : _leds->dim(Drive2);
			_axe->isEffectEnabled(ID_DELAY1)  ? _leds->on(Delay1) : _leds->dim(Delay1);
			_axe->isEffectEnabled(ID_DELAY2)  ? _leds->on(Delay2) : _leds->dim(Delay2);
			_axe->isEffectEnabled(ID_WAH1)    ? _leds->on(Wah1)   : _leds->dim(Wah1);
			_axe->isEffectEnabled(ID_CHORUS1) ? _leds->on(Mod1)   : _leds->dim(Mod1);
		}

		bool filterEffect(const PresetNumber number, AxeEffect effect) {
			switch (effect.getEffectId()) {
				case ID_FUZZ1:
				case ID_FUZZ2:
				case ID_DELAY1:
				case ID_DELAY2:
				case ID_WAH1:
				case ID_CHORUS1:
					return true;
				default:
					return false;
			}
			return false;
		}

	protected:

		enum Buttons {
			PresetSceneUp,    //0
			Drive1,           //1
			Delay1,           //2
			Wah1,             //3
			Tap,              //4
			PresetSceneDown,  //5
			Drive2,           //6
			Delay2,           //7
			Mod1,             //8
			ModeTuner         //9
		};

		using LayoutInterface::LayoutInterface;

		virtual bool readButton(byte index, Button& button) {

			if (!_initialised) {
				_initialised = true;
				setupLeds();
			}

			if (processStandardButtons(index, button)) {
				return true;
			}

			switch(index) {

				case PresetSceneUp:
					if (HOLD) {
						_axe->sendPresetIncrement();
						return true;
					} else if (RELEASE) {
						_axe->sendSceneIncrement();
						return true;
					}
					break;
				
				case PresetSceneDown:
					if (HOLD) {
						_axe->sendPresetDecrement();
						return true;
					} else if (RELEASE) {
						_axe->sendSceneDecrement();
						return true;
					}
					break;

				default:
					return processEffect(index, button);

			};

			return false;

		}

	private:

		void setupLeds() {
			_leds->dimAll();
			_leds->on(PresetSceneUp);
			_leds->on(PresetSceneDown);
		}

		bool processEffect(const byte index, Button& button) {
			EffectId effectId = effectIdFor(index);
			if (HOLD) {
				_axe->sendEffectChannelIncrement(effectId);
				return true;
			} else if (RELEASE) {
				_axe->toggleEffect(effectId);
				return true;
			}
			return false;
		}

		EffectId effectIdFor(const byte index) {
			switch (index) {
				case Drive1: 	return ID_FUZZ1;
				case Drive2:	return ID_FUZZ2;
				case Delay1: 	return ID_DELAY1;
				case Delay2:	return ID_DELAY2;
				case Wah1:		return ID_WAH1;
				case Mod1:		return ID_CHORUS1;
				default: 			return 0; //to stop compiler warnings
			}
		}

		int getEffectOrder(const EffectId effectId) {
			switch (effectId) {
				case ID_FUZZ1:		return 1;
				case ID_FUZZ2:		return 2;
				case ID_DELAY1:		return 3;
				case ID_DELAY2:		return 4;
				case ID_WAH1:			return 5;
				case ID_CHORUS1:	return 6;
			}
			return 0;
		}

		bool _initialised = false;

};
