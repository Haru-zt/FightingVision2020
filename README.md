# FYT2021
 ## 一、运行环境
* 计算设备：MiniPC 或 DJI Manifold 2 (兼容 intel NUC 以及 Nvidia Jetson TX1, TX2）
* 工业相机：迈德威视 MV-SUA134GC-T + MV-LD-8-3M-A
          或者大恒MER-139-210U3C 
* 操作系统：Windows 10 64位 或 Ubuntu 16.04 18.04
* 运行库：OpenCV 3，MindVision相机驱动，大恒相机驱动
* 工具链：
  * Windows 10 : CMake3 + Visual Studio 2019
  * Ubuntu 16.04 : CMake3 + GCC5

相机驱动下载地址：
[MindVision相机驱动](http://www.mindvision.com.cn/rjxz/list_12.aspx?lcid=138)
[Daheng相机驱动](https://www.daheng-imaging.com/Software/index.aspx?nodeid=304)

## 二、程序下载编译运行

* Ubuntu 16.04
```shell
# 下载 FightingVision2020 源码
git clone https://github.com/Haru-zt/FightingVision2020
cd FightingVision2020

# 编译源码
mkdir build
cd build
cmake ..
make

# 运行
sudo ./FightingVision2020
```

* Windows 10

  使用 Visual Studio 2019 打开 CMake 项目，编译运行

  项目结构：
  Armor文件夹主要自瞄的算法部分，需要实现装甲板识别，数字识别（暂时不需要考虑），位置结算等功能
  Camera文件夹为相机驱动
  Config文件夹为配置文件
  SerialPort文件夹为串口通讯
  Tools文件夹为一些部署时的工具
