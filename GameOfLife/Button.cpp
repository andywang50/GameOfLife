#include "Button.h"


SDL_Color Button::get_color() const {
	if (disabled == true) {
		if (hover == true) {
			return Dark_Grey;
		}
		else {
			return Grey;
		}
	}
	else {
		if (hover == true) {
			return Yellow;
		}
		else {
			return Green;
		}
	}
}
/*
Button::Button() {
	_id = ButtonFunc::NONE;
	hover = false;
	disabled = false;
	msg_surface = NULL;
	msg_texture = NULL;

}
Button::~Button() {
	msg_surface = NULL;
	msg_texture = NULL;
}

Button::Button(const Button& b) {
	_id = b._id;
	hover = b.hover;
	disabled = b.disabled;

	msg_surface = b.msg_surface;
	msg_texture = b.msg_texture;
}
Button& Button::operator = (const Button& b) {


	if (this != &b) {
		_id = b._id;
		hover = b.hover;
		disabled = b.disabled;

		msg_surface = b.msg_surface;
		msg_texture = b.msg_texture;
	}
	return *this;
}*/