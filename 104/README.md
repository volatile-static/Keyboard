# True Programmable Keyboard

[![Fork me on Gitee](https://gitee.com/const_volatile/programmable-keyboard/widgets/widget_6.svg)](https://gitee.com/const_volatile/programmable-keyboard)

[中文](https://gitee.com/const_volatile/programmable-keyboard) | English

## Introduction

This project aims to design a high-performance, cool-appearance, and completely customized keyboard, which is specially suitable for CS or EE enthusiasts' further development.

### Finished product

![counter](https://i.loli.net/2021/02/05/KOF3fBxvHteZkUj.gif)

![matrix](preview.gif)

### Parameters

|  |  |
| -------- | --------- |
| total keys | 103+1     |
| keyswitches | 3-pin MX |
| anti-ghosting | full n-key rollover |
| LEDs | 73 monochrome + 31 RGB + lock indicator |
| interface protocol | USB + PS/2 |
| maximum power | 1.35 Watts |
| controller IC | EP4CE6E22C8 |

### Features

- fully programmable: 
  Only use one`Cyclone IV E`family FPGA to control all the on board peripheral. Integrating the software programmability of soft processor with the hardware programmability of FPGA.

- fully open-source: 

  This repository stores the project for FPGA only. **PCB** and other resources are stored in Baidu Netdisk with different type of keyboard, see [home page](https://github.com/volatile-static/Keyboard) for details.

- full n-key rollover: 

  Each switch in matrix connects to a Schottky diode in series, which will realize fundamental full n-key rollover taking advantage of PS/2 protocol.

- fully-parallel LED driver: 

  **168** channels PWM count independently, connecting to Nios II soft core via Avalon-MM bus.

- fully-parallel debouncing: 

  All of the debouncers work simultaneously. See also [Wiki](https://gitee.com/const_volatile/programmable-keyboard/wikis/%E6%95%B0%E5%AD%97%E9%80%BB%E8%BE%91%E8%AE%BE%E8%AE%A1?sort_id=3527145).

- fully-parallel interface: 

  The USB and PS/2 interface can connect to different computer at the same time without any interference. Combining compatibility with performance.

## Transparent blackbox

Normally, the more encapsulation, the fewer details are visible.(like MATLAB) However, bing in charge of low-level details means less development speed.(like Assembly) This project try to work out a compromise that making application development as rapid as possible based on the most transparent.

### Entirely in charge

This is what is called "hardware/software co-design" in a broad sense. The full stack of embedded system includes PCB, digital logic, C and so on. Whichever the level you are, designers are always in charge.

For example, the LED driver solution.  First of all, you should design the scanning mode and select appropriate transistor. Secondly, write  RTL code of matrix scanning according to your schematic. And then realize the digital logic of brightness adjuestment and memory map to soft core. Finally, it's embedded software about RGB color transformation algorithms, etc.

As long as you wish, everything is transparent -- straightaway from a C function to the current in transistors.

### Pretty encapsulation

Although cooperating with hardware and software, mastering every detail is not needed actually.

- If you want to design your own PCB: Draw your PCB based on existing schematics and then burn [firmware](https://gitee.com/const_volatile/programmable-keyboard/releases).
- If you just want to practise soldering: Use PCB documents directly to order PCB prototype. Use BOM to order components. When you finish soldering, burn [firmware](https://gitee.com/const_volatile/programmable-keyboard/releases).
- If you are skilled in mechanics: [Purchase an assembled pcba](https://market.m.taobao.com/app/idleFish-F2e/widle-taobao-rax/page-detail?wh_weex=true&wx_navbar_transparent=true&id=637588964083&ut_sk=1.X7R74tmmaOsDAD0RHKo4TqAp_21407387_1613018144287.Copy.detail.637588964083.2206517679956&forceFlush=1), design your favourite keycap and shell based on existing plate file.
- If you want to use the PS/2 protocol in your own project: Just use these [code](VerilogHDL/ps2_bus.v), [documents](https://blog.csdn.net/weixin_44560710/article/details/112798557) available as well.
- If you are learning SpinalHDL: Just have a look at [code](SpinalHDL/src/main/scala/keyboard)~


## Repository structure

This repo includes four sub-project as below.

| Project                                        | Introduction                                                 |
| ---------------------------------------------- | ------------------------------------------------------------ |
| Quartus                                        | Starting point and destination of the whole project. [Low level code](VerilogHDL/) written in VerilogHDL. |
| [SpinalHDL](SpinalHDL/src/main/scala/keyboard) | Use [open source](https://github.com/SpinalHDL) high level hardware description language to program toplevel logic. |
| Qsys hardware                                  | SOPC designed by Platform Designer.                          |
| [Nios software](Qsys/Software/kbd104)          | Applications written in C language, running in Qsys hardware. |

## Quick start guide

Getting start from [wiki](https://gitee.com/const_volatile/programmable-keyboard/wikis). You can transplant some codes into your project. Or follow these steps to use this project for further development.

1.  Make sure your circuit hardware have no bug.
2.  Install development environment of SpinalHDL and open the [SpinalHDL](SpinalHDL) folder use IDEA.
3.  Run `GenerateIP` and `GenerateTop` in IDEA.
4.  Open Quartus, Platform Designer, generate sopcinfo file.
5.  Start compilation and download sof.
6.  `make` embedded software project to get ELF.
7.  Call Nios Shell，excute [command](Qsys/Software/kbd104/生成固件命令.TXT) in sequence then get HEX file.
8.   Convert hex file to jic file and burn.

