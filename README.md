# CadModelling

《三维CAD建模》2023秋课程作业。

## 环境

Windows 11, Visual Studio 2022, freeglut, GLEW, GLFW, GLM.

C++, MSVC Compiler.

## 使用说明

第三方库已包括在项目文件中，解决方案已配置了相关include和lib路径，理论上可以直接构建启动。

生成的exe添加了一份到项目根目录，可直接启动。

## 操作

包括了两个模型：含三个内环的六边形，以及扫掠得到的闪电形状（凹多边形）。

键盘操作：

- W, S: 相机上下平移
- A, D: 相机拉远拉近
- E: 切换模型

## 代码文件

代码文件全部位于src文件夹中，功能如下：

- main.cpp: 主体，创建窗口并读取模型进行渲染。
- controller.hpp/cpp: 键盘控制。
- data-structure.hpp: 半边数据结构。
- euler-operations.hpp/cpp: 五个欧拉操作及扫掠操作。
- make-models.hpp: 使用各种操作生成多种模型。
- render.hpp/cpp: 渲染。