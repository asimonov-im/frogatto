#include "SDL.h"

#include <iostream>
#include <vector>

#include "foreach.hpp"
#include "joystick.hpp"
#include "preferences.hpp"

namespace joystick {

namespace {
std::vector<SDL_Joystick*> joysticks;

const int threshold = 32700;
}

manager::manager() {
	for(int n = 0; n != SDL_NumJoysticks(); ++n) {
		SDL_Joystick* j = SDL_JoystickOpen(n);
		if(j) {
			joysticks.push_back(j);
		}
	}

	std::cerr << "initialized " << joysticks.size() << " joysticks\n";
}

manager::~manager() {
	foreach(SDL_Joystick* j, joysticks) {
		SDL_JoystickClose(j);
	}
	joysticks.clear();
}

bool connected() {
	if(!preferences::use_joystick()) {
		return false;
	} else {
		return !joysticks.empty();
	}
}

void update() {
	if(preferences::use_joystick()) {
		SDL_JoystickUpdate();
	}
}

bool up() {
	if(!preferences::use_joystick()) {
		return false;
	}

	foreach(SDL_Joystick* j, joysticks) {
		const int nhats = SDL_JoystickNumHats(j);
		for(int n = 0; n != nhats; ++n) {
			const Uint8 state = SDL_JoystickGetHat(j, n);
#ifdef __PLAYBOOK__
			if (n == 1) {
				switch(state) {
				case SDL_HAT_RIGHT:
				case SDL_HAT_RIGHTDOWN:
				case SDL_HAT_RIGHTUP:
					return true;
				}
			}
			else {
				switch(state) {
				case SDL_HAT_UP:
				case SDL_HAT_RIGHTUP:
				case SDL_HAT_LEFTUP:
						return true;
				}
			}
#else
			switch(state) {
			case SDL_HAT_UP:
			case SDL_HAT_RIGHTUP:
			case SDL_HAT_LEFTUP:
					return true;
			}
#endif
		}

	}

	return false;
}

bool down() {
	if(!preferences::use_joystick()) {
		return false;
	}

	foreach(SDL_Joystick* j, joysticks) {
		const int nhats = SDL_JoystickNumHats(j);
		for(int n = 0; n != nhats; ++n) {
			const Uint8 state = SDL_JoystickGetHat(j, n);
#ifdef __PLAYBOOK__
			if (n == 1) {
				switch(state) {
				case SDL_HAT_LEFT:
				case SDL_HAT_LEFTDOWN:
				case SDL_HAT_LEFTUP:
					return true;
				}
			}
			else {
				switch(state) {
				case SDL_HAT_DOWN:
				case SDL_HAT_RIGHTDOWN:
				case SDL_HAT_LEFTDOWN:
						return true;
				}
			}
#else
			switch(state) {
			case SDL_HAT_DOWN:
			case SDL_HAT_RIGHTDOWN:
			case SDL_HAT_LEFTDOWN:
					return true;
			}
#endif
		}

	}

	return false;
}

bool left() {
	if(!preferences::use_joystick()) {
		return false;
	}

	foreach(SDL_Joystick* j, joysticks) {
		const int nhats = SDL_JoystickNumHats(j);
		for(int n = 0; n != nhats; ++n) {
			const Uint8 state = SDL_JoystickGetHat(j, n);
#ifdef __PLAYBOOK__
			if (n == 1) {
				switch(state) {
				case SDL_HAT_UP:
				case SDL_HAT_LEFTUP:
				case SDL_HAT_RIGHTUP:
					return true;
				}
			}
			else {
				switch(state) {
				case SDL_HAT_LEFT:
				case SDL_HAT_LEFTDOWN:
				case SDL_HAT_LEFTUP:
						return true;
				}
			}
#else
			switch(state) {
			case SDL_HAT_LEFT:
			case SDL_HAT_LEFTDOWN:
			case SDL_HAT_LEFTUP:
					return true;
			}
#endif
		}

	}

	return false;
}

bool right() {
	if(!preferences::use_joystick()) {
		return false;
	}

	foreach(SDL_Joystick* j, joysticks) {
		const int nhats = SDL_JoystickNumHats(j);
		for(int n = 0; n != nhats; ++n) {
			const Uint8 state = SDL_JoystickGetHat(j, n);
#ifdef __PLAYBOOK__
			if (n == 1) {
				switch(state) {
				case SDL_HAT_DOWN:
				case SDL_HAT_LEFTDOWN:
				case SDL_HAT_RIGHTDOWN:
					return true;
				}
			}
			else {
				switch(state) {
				case SDL_HAT_RIGHT:
				case SDL_HAT_RIGHTDOWN:
				case SDL_HAT_RIGHTUP:
						return true;
				}
			}
#else
			switch(state) {
			case SDL_HAT_RIGHT:
			case SDL_HAT_RIGHTDOWN:
			case SDL_HAT_RIGHTUP:
					return true;
			}
#endif
		}
	}

	return false;
}

bool button(int n) {
	if(!preferences::use_joystick()) {
		return false;
	}

#ifdef __PLAYBOOK__
	if (joysticks.size() > 0) {
		switch (n) {
		case 0:
			if (SDL_JoystickGetButton(joysticks[0], 0) // WiiMote A
					|| SDL_JoystickGetButton(joysticks[0], 7)) // Classic Controller a
				return true;
			break;
		case 1:
			if (SDL_JoystickGetButton(joysticks[0], 2) // WiiMote 1
					|| SDL_JoystickGetButton(joysticks[0], 8)) // Classic Controller b
				return true;
			break;
		case 2:
			if (SDL_JoystickGetButton(joysticks[0], 3) // WiiMote 2
					|| SDL_JoystickGetButton(joysticks[0], 17) // Classic Controller R
					|| SDL_JoystickGetButton(joysticks[0], 15)) // Classic Controller zR
				return true;
			break;
		}
	}
#else
	foreach(SDL_Joystick* j, joysticks) {
		if(SDL_JoystickGetButton(j, n)) {
			return true;
		}
	}
#endif

	return false;
}

int iphone_tilt() {
//#if TARGET_OS_IPHONE
//	return SDL_JoystickGetAxis(joysticks.front(), 1);
//#else
	return 0;
//#endif
}

std::vector<int> get_info() {
	std::vector<int> res;
	res.push_back(joysticks.size());
	foreach(SDL_Joystick* j, joysticks) {
		res.push_back(SDL_JoystickGetAxis(j, 0));
		res.push_back(SDL_JoystickGetAxis(j, 1));
	}
	
	return res;
}

}
