
# 基于OPENCV的视频监控
项目已经实现用户自由创建多个线程，实时更改线程参数；每个线程都可以执行对本地视频或者摄像头视频的危险区域检测，并将入侵的视频片段录制保存。
## 运行环境
VS2017+opencv3.4+boost1.67
## 使用说明
安装配置好环境之后，运行程序，出现命令行交互界面：

   ![交互](https://raw.githubusercontent.com/TSijia/git_test/master/CMD.png)

- **设置参数表说明：**
    -  **threadID :** 每一个创建的线程有唯一的ID号，由用户在创建的时指定，用户在进行任何操作时都要指明被操作线程的ID,默认值为-1
    -  **threadOP :** 可选项 “open”和“close”，改变已创建线程的参数不必输入该命令，默认值为“none”
    -  **CameraLabel :** 摄像头的索引，一般为0、1、2...,默认值为-1
    -  **filePath :** 本地视频的路径，如果没有给定规定的摄像头索引，则处理本地视频，默认值“luren.avi”
    -  **areaID :** 用户设定画面中需要检测的区域，默认值为111111
 
        <div align="center" title = "area_id">
        <img src="https://raw.githubusercontent.com/TSijia/git_test/master/绘图1.jpg"  alt="模型" align=center  />
        </div>

    -  **edgeNUM :** 用户设定布防区域的边数（四边形、五边形...）,相应的布放区域的坐标信息保存在程序中，默认值为4（目前只保存的四边形的坐标信息）
    -  **method ：** 程序中进行背景分割的方法，目前可选项"knn"和"mog2",默认为"knn"
-  **用户指令示例：**
    - **创建线程：**   
     1. **threadID X threadOP open** ：使用默认参数创建线程X，对本地视频“luren1.avi”进行检测，布放区域为六个四边形并且对每个布放区域都要检测，背景分割方法为knn
     2. **threadID X threadOP open CamreLabel 0 areaID 100000 method mog2** : 创建 线程X，对摄像头视频进行处理并且除了edgeNUM外其余参数都做了更改
    - **创建线程后更改参数**
     1. **threadID X areaID 100000** : 必须指明线程ID，然后对参数表中参数进行更改
    - **关闭线程**
     1. **threadID X threadOP close**
    - **退出进程**
     1. **quit**
## 运行结果
- 本地视频
  1. 检测结果

        <div align="center" >
        <img src="https://raw.githubusercontent.com/TSijia/git_test/master/人.png"  alt="模型" align=center  />
        </div>

  2. 保存入侵视频(示例)
          <div align="center" >
        <img src="https://raw.githubusercontent.com/TSijia/git_test/master/0_2018_6_24-19_50_26.gif"  alt="模型" align=center  />
        </div>


- 摄像头
  1. 检测结果

        <div align="center" >
        <img src="https://raw.githubusercontent.com/TSijia/git_test/master/me.png"  alt="模型" align=center  />
        </div>
  2. 保存入侵视频(示例)
            <div align="center" >
        <img src="https://raw.githubusercontent.com/TSijia/git_test/master/0_2018_6_26-18_27_40-2.gif"  alt="模型" align=center  />
        </div>
- 结果保存
  1. 在dangerVideo文件夹中对应的线程ID子文件夹中保存着截取的入侵视频
## 程序框架
- [线程管理（Controller + Worker） + 布防区域检测报警](./src/README.md)
