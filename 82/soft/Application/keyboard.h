#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "delay.h"
#include "pin_def.h"

struct TKeys {
	  void Send();
};

class TLeds {	
		friend class TKeys;
		void Update();
public:
		void SetAll(bool On);
		void Random();
		void FeedBack();
};

void keyboard_init();

#endif
