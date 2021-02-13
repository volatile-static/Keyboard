# 可编程机械键盘

中文 | [English](https://github.com/volatile-static/Keyboard/tree/master/104)

## 介绍

本项目旨在设计一款专为CS和EE发烧友二次开发的高性能、高颜值、高自由度的客制化键盘。

### 成品效果图

![counter](https://i.loli.net/2021/02/05/KOF3fBxvHteZkUj.gif)

![matrix](preview.gif)

### 参数

|  |  |
| -------- | --------- |
| 按键数量 | 103+1     |
| 按键类型 | 3脚机械轴  |
| 扫描方式   | 行列式 |
| 全键无冲 | 是 |
| 灯光 | 73单色+31全彩+锁定键指示灯 |
| 接口 | USB+PS/2 |
| 最大功率 | 1.35瓦特 |
| 主控芯片 | EP4CE6E22C8 |

### 特性

- 完全可编程：仅采用一片`Cyclone IV E`系列FPGA控制全部板载外设，*软硬件协同开发*。
- 完全开源：本仓库仅存放FPGA工程，**PCB**等资源与其他型号键盘一同存放在百度云，详见[GitHub主页](https://github.com/volatile-static/Keyboard)。
- 全键无冲：矩阵键盘中每一个按键都串联一只二极管，配合PS/2协议实现“真·全键无冲”。
- 全并行LED驱动：**168**路PWM独立计数，通过Avalon-MM总线连接Nios II软核，详见[Wiki](https://gitee.com/const_volatile/programmable-keyboard/wikis/%E6%95%B0%E5%AD%97%E9%80%BB%E8%BE%91%E8%AE%BE%E8%AE%A1?sort_id=3527145)。
- 全并行按键消抖：扫描后为每个键独立延时消抖。
- 全并行接口：USB与PS/2接口可同时连接不同电脑，互不干扰，兼顾高性能与兼容性。

## 透明封装

通常，封装程度越高意味着可访问的底层细节越少（比如MATLAB），而对底层的完全掌控意味着更低的开发效率（比如汇编语言）。本项目试图采取一个折中的方案，即在保证足够的透明程度下尽量减小应用开发的复杂度。

### 完全掌控

这是广义的“软硬件协同开发”，从C语言到数字逻辑、再到电路板“全栈”开发。比如LED驱动方案，首先要设计扫描方式，然后对晶体管选型；设计出pcb后要对照FPGA引脚写出矩阵扫描的RTL代码，进而实现调节亮度的数字逻辑、连接到软核的内存映射，最后软件实现RGB色彩变换的算法等。只要你愿意，一切原理都是透明的——从C语言调用一个函数直通晶体管的电流方向。

### 高度封装

虽然是“软硬件协同开发”，但其实并不需要你掌握每一个细节。

- 如果你想设计自己的电路板：完全可以拿现有的原理图，画出自己的PCB，然后烧录[固件](https://gitee.com/const_volatile/programmable-keyboard/releases)
- 如果你只想练习焊接：直接用PCB文件去打样，照着BOM买齐元器件，焊完烧录即可
- 如果你对机械更感兴趣：[买个成品PCB](https://market.m.taobao.com/app/idleFish-F2e/widle-taobao-rax/page-detail?wh_weex=true&wx_navbar_transparent=true&id=637588964083&ut_sk=1.X7R74tmmaOsDAD0RHKo4TqAp_21407387_1613018144287.Copy.detail.637588964083.2206517679956&forceFlush=1)，在已有的[定位板文件]基础上设计自己的外壳或键帽吧
- 如果你只有一点单片机基础：同样可以[买个成品PCB](https://market.m.taobao.com/app/idleFish-F2e/widle-taobao-rax/page-detail?wh_weex=true&wx_navbar_transparent=true&id=637588964083&ut_sk=1.X7R74tmmaOsDAD0RHKo4TqAp_21407387_1613018144287.Copy.detail.637588964083.2206517679956&forceFlush=1)，学习一下FPGA，更好地理解单片机原理
- 如果你想在自己的项目里使用PS/2协议：直接把[代码](VerilogHDL/ps2_bus.v)拿去用，这里还有[教程](https://blog.csdn.net/weixin_44560710/article/details/112798557)
- 如果你正在学习SpinalHDL：什么也不用做，看看[代码](SpinalHDL/src/main/scala/keyboard)得了~


## 仓库结构

本仓库包含以下四个子工程。

| 工程                                           | 说明                                                         |
| ---------------------------------------------- | ------------------------------------------------------------ |
| Quartus                                        | 整个项目的出发点和落脚点。[底层代码](VerilogHDL/)采用Verilog语言编写。 |
| [SpinalHDL](SpinalHDL/src/main/scala/keyboard) | 使用[开源](https://github.com/SpinalHDL)的高级硬件描述语言编写顶层逻辑。 |
| Qsys硬件                                       | 使用Platform Designer设计的片上可编程系统。（SOPC）          |
| [Nios软件](Qsys/Software/kbd104)               | 采用C语言实现的应用层，运行在Qsys硬件中。                    |

## 使用说明

入门请看[wiki](https://gitee.com/const_volatile/programmable-keyboard/wikis)。你可以将本项目中的部分代码移植到你的项目中，也可以按以下步骤使用本项目二次开发。

1.  确保电路硬件无bug
2.  安装好SpinalHDL开发环境并用IDEA打开SpinalHDL文件夹
3.  在IDEA中运行GenerateIP与GenerateTop
4.  打开Quartus，Platform Designer，生成sopcinfo文件
5.  全综合，得到sof并下载
6.  make软件工程得到ELF
7.  打开Nios Shell，依次执行[命令](Qsys/Software/kbd104/生成固件命令.TXT)得到hex文件
8.   将hex文件转换为jic文件并烧录

## 联系方式

- QQ：1043064987
- 邮箱：`__volatile__@csu.edu.cn`

