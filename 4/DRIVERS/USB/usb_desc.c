#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Extern variables ---------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/* USB Standard Device Descriptor */
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] = {
	0x12, /* bLength */ USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */ 0x00,
	/* bcdUSB */ 0x02, 0x00, /* bDeviceClass */ 0x00,
	/* bDeviceSubClass */ 0x00, /* bDeviceProtocol */ 0x40,
	/* bMaxPacketSize40 */ 0x93, /* idVendor (0x0483) */ 0x19, 0x03,
	/* idProduct = 0x5710 */ 0x11, 0x00, /* bcdDevice rel. 2.00 */ 0x02, 1,
	/* Index of string descriptor describing manufacturer */ 2,
	/* Index of string descriptor describing product */ 3,
	/* Index of string descriptor describing the device serial number */ 0x01
		/* bNumConfigurations */ }; /* Joystick_DeviceDescriptor */

/* USB Configuration Descriptor */
/* All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] = {
	0x09, /* bLength: Configuation Descriptor size */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,
	/* bDescriptorType: Configuration */ JOYSTICK_SIZ_CONFIG_DESC,
	/* wTotalLength: Bytes returned */ 0x00, 0x01,
	/* bNumInterfaces: 1 interface */ 0x01,
	/* bConfigurationValue: Configuration value */ 0x00,
	/* iConfiguration: Index of string descriptor describing
	 the configuration */ 0xC0, /* bmAttributes: self powered */ 0x32,
	/* MaxPower 100 mA: this current is used for detecting Vbus */
	// 以下为接口描述符
	/************** Descriptor of Joystick Mouse interface *************** */
	/* 09 */ 0x09,
	/* bLength: Interface Descriptor size */ USB_INTERFACE_DESCRIPTOR_TYPE,
	/* bDescriptorType: Interface descriptor type */ 0x00,
	/* bInterfaceNumber: Number of Interface */ 0x00,
	/* bAlternateSetting: Alternate setting */ 0x02, /* bNumEndpoints */ 0x03,
	/* bInterfaceClass: HID */ 0x01,
	/* bInterfaceSubClass : 1=BOOT, 0=no boot */ 0x01,
	/* bInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */ 0,
	/* iInterface: Index of string descriptor */
	// 以下为HID描述符
	/******************** Descriptor of Joystick Mouse HID ******************* */
	/* 18 */ 0x09, /* bLength: HID Descriptor size */ HID_DESCRIPTOR_TYPE,
	/* bDescriptorType: HID */ 0x00,
	/* bcdHID: HID Class Spec release number */ 0x01, 0x00,
	/* bCountryCode: Hardware target country */ 0x01,
	/* bNumDescriptors: Number of HID class descriptors to follow */ 0x22,
	/* bDescriptorType */ JOYSTICK_SIZ_REPORT_DESC,
	/* wItemLength: Total length of Report descriptor */ 0x00,

	// 以下为输入端点1描述符
	/******************** Descriptor of Joystick Mouse endpoint ******************* */
	/* 27 */ 0x07,
	/* bLength: Endpoint Descriptor size */ USB_ENDPOINT_DESCRIPTOR_TYPE,
	/* bDescriptorType: */ 0x81,
	/* bEndpointAddress: Endpoint Address (IN) */ 0x03,
	/* bmAttributes: Interrupt endpoint */ 0x08,
	/* wMaxPacketSize: 8 Byte max */ 0x00, 0x20,
	/* bInterval: Polling Interval (32 ms) */
	// 以下为输出端但1描述符
	/* 34 */ 0x07,
	/* bLength: Endpoint Descriptor size */ USB_ENDPOINT_DESCRIPTOR_TYPE,
	/* bDescriptorType: */ 0x01,
	/* bEndpointAddress: Endpoint Address (OUT) */ 0x03,
	/* bmAttributes: Interrupt endpoint */ 0x08,
	/* wMaxPacketSize: 8 Byte max */ 0x00, 0x20}; /* MOUSE_ConfigDescriptor */

const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] = {
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
//表示用途为键盘
0x09, 0x06, // USAGE (Keyboard)

//表示应用集合，必须要以END_COLLECTION来结束它，见最后的END_COLLECTION
0xa1, 0x01, // COLLECTION (Application)

//表示用途页为按键
0x05, 0x07, // USAGE_PAGE (Keyboard)
//用途最小值，这里为左ctrl键
0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
//用途最大值，这里为右GUI键，即window键
0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
//逻辑最小值为0
0x15, 0x00, // LOGICAL_MINIMUM (0)
//逻辑最大值为1
0x25, 0x01, // LOGICAL_MAXIMUM (1)
//报告大小（即这个字段的宽度）为1bit，所以前面的逻辑最小值为0，逻辑最大值为1
0x75, 0x01, // REPORT_SIZE (1)
//报告的个数为8，即总共有8个bits
0x95, 0x08, // REPORT_COUNT (8)
//输入用，变量，值，绝对值。像键盘这类一般报告绝对值，
//而鼠标移动这样的则报告相对值，表示鼠标移动多少
0x81, 0x02, // INPUT (Data,Var,Abs)
//上面这这几项描述了一个输入用的字段，总共为8个bits，每个bit表示一个按键
//分别从左ctrl键到右GUI键。这8个bits刚好构成一个字节，它位于报告的第一个字节。
//它的最低位，即bit-0对应着左ctrl键，如果返回的数据该位为1，则表示左ctrl键被按下，
//否则，左ctrl键没有按下。最高位，即bit-7表示右GUI键的按下情况。中间的几个位，
//需要根据HID协议中规定的用途页表（HID Usage Tables）来确定。这里通常用来表示
//特殊键，例如ctrl，shift，del键等

//这样的数据段个数为1
0x95, 0x01, // REPORT_COUNT (1)
//每个段长度为8bits
0x75, 0x08, // REPORT_SIZE (8)
//输入用，常量，值，绝对值
0x81, 0x03, // INPUT (Cnst,Var,Abs)

//上面这8个bit是常量，设备必须返回0

//这样的数据段个数为5
0x95, 0x05, // REPORT_COUNT (5)
//每个段大小为1bit
0x75, 0x01, // REPORT_SIZE (1)
//用途是LED，即用来控制键盘上的LED用的，因此下面会说明它是输出用
0x05, 0x08, // USAGE_PAGE (LEDs)
//用途最小值是Num Lock，即数字键锁定灯
0x19, 0x01, // USAGE_MINIMUM (Num Lock)
//用途最大值是Kana，这个是什么灯我也不清楚^_^
0x29, 0x05, // USAGE_MAXIMUM (Kana)
//如前面所说，这个字段是输出用的，用来控制LED。变量，值，绝对值。
//1表示灯亮，0表示灯灭
0x91, 0x02, // OUTPUT (Data,Var,Abs)
//这样的数据段个数为1
0x95, 0x01, // REPORT_COUNT (1)
//每个段大小为3bits
0x75, 0x03, // REPORT_SIZE (3)
//输出用，常量，值，绝对
0x91, 0x03, // OUTPUT (Cnst,Var,Abs) 
//由于要按字节对齐，而前面控制LED的只用了5个bit，
//所以后面需要附加3个不用bit，设置为常量。

//报告个数为6
0x95, 0x06, // REPORT_COUNT (6)
//每个段大小为8bits
0x75, 0x08, // REPORT_SIZE (8)
//逻辑最小值0
0x15, 0x00, // LOGICAL_MINIMUM (0)
//逻辑最大值255
0x25, 0xFF, // LOGICAL_MAXIMUM (255)
//用途页为按键
0x05, 0x07, // USAGE_PAGE (Keyboard)
//使用最小值为0
0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
//使用最大值为0x65
0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
//输入用，变量，数组，绝对值
0x81, 0x00, // INPUT (Data,Ary,Abs)
//以上定义了6个8bit宽的数组，每个8bit（即一个字节）用来表示一个按键，所以可以同时
//有6个按键按下。没有按键按下时，全部返回0。如果按下的键太多，导致键盘扫描系统
//无法区分按键时，则全部返回0x01，即6个0x01。如果有一个键按下，则这6个字节中的第一
//个字节为相应的键值（具体的值参看HID Usage Tables），如果两个键按下，则第1、2两个

//	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
//	0x09, 0x06, // USAGE (Keyboard)
//	0xa1, 0x01, // COLLECTION (Application)
//	0x05, 0x07, // USAGE_PAGE (Keyboard/Keypad)
//	0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
//	0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
//	0x15, 0x00, // LOGICAL_MINIMUM (0)
//	0x25, 0x01, // LOGICAL_MAXIMUM (1)
//	0x95, 0x08, // REPORT_COUNT (8)
//	0x75, 0x01, // REPORT_SIZE (1)
//	0x81, 0x02, // INPUT (Data,Var,Abs)
//	0x95, 0x01, // REPORT_COUNT (1)
//	0x75, 0x08, // REPORT_SIZE (8)
//	0x81, 0x03, // INPUT (Cnst,Var,Abs)
//	0x95, 0x06, // REPORT_COUNT (6)
//	0x75, 0x08, // REPORT_SIZE (8)
//	0x25, 0xFF, // LOGICAL_MAXIMUM (255)
//	0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
//	0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
//	0x81, 0x00, // INPUT (Data,Ary,Abs)
//	0x25, 0x01, // LOGICAL_MAXIMUM (1)
//	0x95, 0x05, // REPORT_COUNT (5)
//	0x75, 0x01, // REPORT_SIZE (1)
//	0x05, 0x08, // USAGE_PAGE (LEDs)
//	0x19, 0x01, // USAGE_MINIMUM (Num Lock)
//	0x29, 0x02, // USAGE_MAXIMUM (Caps Lock)
//	0x91, 0x02, // OUTPUT (Data,Var,Abs)
//	0x95, 0x01, // REPORT_COUNT (1)
//	0x75, 0x06, // REPORT_SIZE (6)
//	0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
	0xc0 // END_COLLECTION
}; /* Joystick_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] = {
	JOYSTICK_SIZ_STRING_LANGID, USB_STRING_DESCRIPTOR_TYPE, 0x09, 0x04
}; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] = {
	JOYSTICK_SIZ_STRING_VENDOR,
	/* Size of Vendor string */ USB_STRING_DESCRIPTOR_TYPE,
	/* bDescriptorType */
	/* Manufacturer: "STMicroelectronics" */ 'S', 0, 'T', 0, 'M', 0, 'i', 0,
	'c', 0, 'r', 0, 'o', 0, 'e', 0, 'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o',
	0, 'n', 0, 'i', 0, 'c', 0, 's', 0};

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] = {
	JOYSTICK_SIZ_STRING_PRODUCT, /* bLength */ USB_STRING_DESCRIPTOR_TYPE,
	/* bDescriptorType */ 'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'K',
	0, 'e', 0, 'y', 0, 'B', 0, 'o', 0, 'a', 0, 'r', 0, 'd', 0};
uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] = {
	JOYSTICK_SIZ_STRING_SERIAL, /* bLength */ USB_STRING_DESCRIPTOR_TYPE,
	/* bDescriptorType */ 'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
