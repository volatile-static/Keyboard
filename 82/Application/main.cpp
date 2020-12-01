#include "keyboard.h"

int main() {
	keyboard_init();
	TKeys Key;
	TLeds Led;
	
	Led.SetAll(true);
	for (; ;) {
		Key.Send();
	}	
}
