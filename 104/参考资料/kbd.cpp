/*
	PS/2 library for Arduino

	Copyright (c) 2014 Freedelity

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
 */

#include "FidPS2Host.h"
#include "utility/fid_timer.h"
#include "utility/fid_circular_buffer.h"

static void fid_ps2h_release_pins();
static void fid_ps2h_attach_isr();
static void fid_ps2h_clock_isr();

static int mClockPin = 2;
static int mDataPin = 4;
static int mIrqNum = 0;

static int clock = HIGH;

#define HOST_CLOCK_SET_MASK 0x04
#define HOST_CLOCK_CLEAR_MASK 0xFB
#define HOST_CLOCK_INPUT_MASK 0xFB
#define HOST_CLOCK_OUTPUT_MASK 0x04
#define HOST_DATA_SET_MASK 0x10
#define HOST_DATA_CLEAR_MASK 0xEF
#define HOST_DATA_INPUT_MASK 0xEF
#define HOST_DATA_OUTPUT_MASK 0x10

volatile static bool reading = false;
volatile static bool writing = false;
static int readwrite_count = 0;
static uint8_t byte_read = 0;
volatile static bool odd_parity = false;
static uint8_t byte_to_write = 0;

static CircularBuffer writeBuffer;
static CircularBuffer readBuffer;

void fid_ps2h_init(int dataPin, int clockPin)
{
	mClockPin = clockPin;
	mDataPin = dataPin;
	mIrqNum = -1;
	
	if( mClockPin == 2 )
		mIrqNum = 0;
	else if( mClockPin == 3 )
		mIrqNum = 1;
	
	writeBuffer.clear();
	readBuffer.clear();
	
	fid_ps2h_release_pins();
	
	fid_ps2h_attach_isr();
	
	reading = false;
	writing = false;
	readwrite_count = 0;
	byte_to_write = 0;
	byte_read = 0;
	odd_parity = false;
	
	fid_timer_init();
	//fid_timer_add_function(fid_ps2h_clk_20us);
}

static inline void fid_ps2h_release_pin(int pin)
{
	DDRD &= ~(1<<pin); // input
	PORTD |= (1<<pin); // high: pull-up resistor
}

static inline void fid_ps2h_set_pin_low(int pin)
{
	DDRD |= (1<<pin); // output
	PORTD &= ~(1<<pin); // low
}

static inline void fid_ps2h_set_clock_high()
{
#ifndef HOST_CLOCK_SET_MASK
	fid_ps2h_release_pin(mClockPin);
#else
	DDRD &= HOST_CLOCK_INPUT_MASK;
	PORTD |= HOST_CLOCK_SET_MASK;
#endif
}

static inline void fid_ps2h_set_clock_low()
{
#ifndef HOST_CLOCK_SET_MASK
	fid_ps2h_set_pin_low(mClockPin);
#else
	DDRD |= HOST_CLOCK_OUTPUT_MASK;
	PORTD &= HOST_CLOCK_CLEAR_MASK;
#endif
}

static inline void fid_ps2h_set_data_high()
{
#ifndef HOST_DATA_SET_MASK
	fid_ps2h_release_pin(mDataPin);
#else
	DDRD &= HOST_DATA_INPUT_MASK;
	PORTD |= HOST_DATA_SET_MASK;
#endif
}

static inline void fid_ps2h_set_data_low()
{
#ifndef HOST_DATA_SET_MASK
	fid_ps2h_set_pin_low(mDataPin);
#else
	DDRD |= HOST_DATA_OUTPUT_MASK;
	PORTD &= HOST_DATA_CLEAR_MASK;
#endif
}

static inline void fid_ps2h_release_pins()
{
	fid_ps2h_release_pin(mDataPin);
	fid_ps2h_release_pin(mClockPin);
}

static void fid_ps2h_attach_isr()
{
	if( mIrqNum != -1 )
	{
		detachInterrupt(mIrqNum);
		//attachInterrupt(mIrqNum, fid_ps2h_isr, RISING);
		attachInterrupt(mIrqNum, fid_ps2h_clock_isr, CHANGE);
	}
}

void fid_ps2h_write(uint8_t b)
{
	noInterrupts();
	writeBuffer.put(b);
	interrupts();
}

bool fid_ps2h_read(uint8_t* b)
{
	noInterrupts();
	bool r = readBuffer.read(b);
	interrupts();
	return r;
}

static void fid_ps2h_clock_isr()
{
#ifndef HOST_CLOCK_SET_MASK
	clock = PIND & (1<<mClockPin);
#else
	clock = PIND & HOST_CLOCK_SET_MASK;
#endif
}

void fid_ps2h_clk_20us()
{
#ifndef HOST_DATA_SET_MASK
	int data = PIND & (1<<mDataPin);
#else
	int data = PIND & HOST_DATA_SET_MASK;
#endif
	static unsigned int isr_count = 0;
	static bool nextLastClock = false;
	
	if( !writing && !reading )
	{
		if( data != 0 && clock != 0 )
		{
			// line is idle, can send next byte if any
			if( !writeBuffer.empty() )
			{
				writeBuffer.read(&byte_to_write);
				isr_count = 0;
				readwrite_count = 0;
				writing = true;
			}
		}
		else if( data == 0 && clock != 0 )
		{
			// host want to send a byte
			isr_count = 0;
			readwrite_count = 0;
			reading = true;
		}
		else
		{
			fid_ps2h_set_clock_high();
		}
		
	}
	else if( writing )
	{
		if( isr_count == 0 ) // write bit
		{
			if( readwrite_count == 0 ) // start bit
			{
				odd_parity = false;
				nextLastClock = false;
				fid_ps2h_set_data_low();
			}
			else if( readwrite_count <= 8 ) // data bit
			{
				int b = (byte_to_write >> (readwrite_count -1)) & 0x01;
				if( b != 0 )
				{
					fid_ps2h_set_data_high();
					odd_parity = !odd_parity;
				}
				else
				{
					fid_ps2h_set_data_low();
				}
			}
			else if( readwrite_count == 9 ) // parity bit
			{
				if( odd_parity )
					fid_ps2h_set_data_low();
				else
					fid_ps2h_set_data_high();
			}
			else if( readwrite_count == 10 ) // stop bit = release data
			{
				fid_ps2h_set_data_high();
			}
			else
			{
				nextLastClock = true;
			}
			
			readwrite_count++;
		}
		else if( isr_count == 1 ) // clock low
		{
			fid_ps2h_set_clock_low();
		}
		else if( isr_count == 2 )
		{
			// do nothing, host is sampling data
		}
		else
		{
			fid_ps2h_set_clock_high();
			if( nextLastClock )
			{
				writing = false;
				nextLastClock = false;
			}
		}
		
		if( ++isr_count > 3 )
			isr_count = 0;
	}
	else // reading
	{
		
		if( isr_count == 0 ) // clock low
		{
			fid_ps2h_set_clock_low();
		}
		else if( isr_count == 1 )
		{
			//do nothing, host is setting bit
		}
		else if( isr_count == 2 ) // clock hi
		{
			fid_ps2h_set_clock_high();
		}
		else // sample bit
		{
			if( readwrite_count == 0 ) // first bit
			{
				if( data != 0 )
				{
					byte_read = 1;
					odd_parity = true;
				}
				else
				{
					byte_read = 0;
					odd_parity = false;
				}
			}
			else if( readwrite_count < 8 )
			{
				if( data != 0 )
				{
					byte_read |= (1 << readwrite_count);
					odd_parity = !odd_parity;
				}
			}
			else if( readwrite_count == 8 ) // parity bit
			{
				if( data != 0 )
				{
					odd_parity = ! odd_parity;
				}
			}
			else if( readwrite_count == 9 ) // stop bit + ack
			{
				fid_ps2h_set_data_low();
			}
			else
			{
				readBuffer.put(byte_read);
				// read over, release data
				fid_ps2h_set_data_high();
				reading = false;
			}
			
			readwrite_count++;
		}
		
		if( ++isr_count > 3 )
			isr_count = 0;
	}
}
