FYAIRO 机器人视觉控制系统
FYAIRO 是一个面向机器人平台的综合视觉与控制框架，集成了立体视觉、人脸/物体/棋子识别、机械臂运动控制、网络通信、语音交互和 3D 可视化等功能。支持 ARM 嵌入式平台、服务器和桌面环境，适用于智能机器人、围棋机器人等场景。

功能特性
立体视觉：双摄像头图像采集、去畸变、立体校正、深度计算。

视觉识别：

人脸检测与识别（Dlib、Eigenfaces）

通用物体识别（ResNet）

围棋棋盘与棋子识别

二维码扫描

运动控制：

多自由度机械臂逆运动学求解

头部、手臂、腿部关节控制

动作编排与执行（打招呼、抓取、下棋等）

网络通信：

UDP/TCP 双协议支持

云端与局域网视频传输、指令转发

多机器人协同

语音交互：音频采集、MFCC 特征提取、语音播报。

3D 可视化：基于 OpenGL/GLUT 的实时场景渲染、模型加载与交互。

多线程架构：图像采集、识别、控制、通信等任务并行处理。

目录结构
text
.
├── CMakeLists.txt          # CMake 构建脚本
├── main.cpp                # 主程序入口，GLUT 初始化与主循环
├── i.cpp                   # 核心控制线程与业务逻辑
├── thread000.cpp           # 网络、视频、识别等线程实现
├── CRC16.cpp               # CRC16 校验与命令确认
├── globjld.cpp             # OBJ 模型加载与 OpenGL 绘制
├── InverseDynamics.cpp     # 逆运动学计算
├── eye/                    # 视觉与感知模块
│   ├── readfrm/            # 图像读取与传输（socket、Camera）
│   ├── dfrc/               # 人脸识别
│   ├── facedection/        # 人脸检测
│   ├── mov/                # 移动侦测
│   ├── cvface2/            # OpenCV 人脸识别
│   └── ...
├── mind/                   # 机器人运动学与决策
│   ├── local/              # 局部规划
│   ├── 3d/                 # 3D 几何与视觉
│   └── ...
├── d2/                     # 立体视觉相关
├── fycrc/                  # CRC 校验库
└── ear/                    # 音频处理
依赖环境
CMake >= 3.10.2

C++ 编译器：支持 C++11（gcc/g++）

OpenCV：核心、imgproc、highgui、videoio、dnn、objdetect、face 等模块

dlib：人脸检测与识别

OpenGL / GLUT / GLU：3D 可视化

zbar：二维码识别

FreeType2：文字渲染

ALSA (asound)：音频采集与播放

pthread：多线程支持

编译构建
项目使用 CMake 构建，根据编译器自动选择目标平台：

ARM 平台：arm-linux-gnueabihf-gcc → 生成 FYAIRO_arm

服务器平台：gcc-7 → 生成 FYAIRO_server

桌面平台：默认 gcc → 生成 FYAIRO_desktop

编译步骤
bash
mkdir build && cd build
cmake ..
make -j4
编译成功后，可执行文件位于 build/ 目录下。

运行说明
bash
./FYAIRO_desktop [选项]
选项：

-c：启用云模式（连接云端服务器）

运行前需确保以下文件/路径存在：

配置文件：/home/root/eye_config.yml

人脸数据：/home/root/eye/dt/face.yml

相机标定文件：/home/root/eye/dt/em/eyematrix

3D 模型与纹理：/home/root/eye/ms/

字体文件：/home/root/eye/方正黑体简体.TTF

若首次运行，程序可自动生成默认配置文件（参考 thread000.cpp 中的 fystart()）。

配置说明
eye_config.yml 包含网络参数、摄像头分辨率、IP 地址等，关键配置项：

yaml
MyName: "小飞"
Dv: 2                # 运行模式：0-低功耗，1-中，2-高
Dc: 0                # 是否云端
tsmd: "tcp"          # 视频传输协议
ewd: 640             # 图像宽度
eht: 480             # 图像高度
PcIp: "192.168.8.110"
RobotIp: "192.168.8.11"
CloudIp: "106.75.217.253"
...
主要线程与功能
线程 ID	名称	功能
0	视频采集线程	摄像头图像采集与分发
2	信息打印	状态显示
6	二维码识别	zbar 二维码扫描
8	说话	语音合成与播报
16	空间感知	人脸/物体/棋子空间定位
17	通讯线程	舵机指令发送
18	识别人并打招呼	人脸跟随与打招呼
19	打招呼与告别	手臂动作编排
20	抓取工作	机械臂抓取逆运动学控制
21	下围棋	围棋对弈流程
22	目标跟随	头部/身体对准目标
23	动作控制	关节状态更新与同步
24/28/29	控制线程	运动学模型更新
27	棋盘识别	围棋棋盘网格识别
30/31	人脸检测左/右	ResNet 人脸检测
32/33	物体识别左/右	ResNet 物体检测
34/35	棋子识别左/右	围棋棋子检测
88	听觉	音频采集与频谱分析
100/101	人脸识别左/右	人脸特征提取与识别
许可证
本项目未指定开源许可证，请遵循项目内相关说明或联系作者。
捐赠BTC: 13SongiriQuWoFhoimsVS21CyaTxozKBVA
