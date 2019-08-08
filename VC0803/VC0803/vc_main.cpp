#include <iostream>
#include <windows.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	// 读取视频或摄像头
	//VideoCapture capture("视频路径\\文件名.mp4");
	// 读取摄像头
	//VideoCapture capture0(0);	// 此处0是PC的摄像头
	VideoCapture capture(1);	// 此处1是外接的摄像头

	// 检测是否正常打开:成功打开时，isOpened返回ture
	if (!capture.isOpened()){
		cout << "fail to open!" << endl;
		return -1;
	}
	

	while (1){
		//Mat frame0;
		//capture0 >> frame0;
		//imshow("video0", frame0);

		Mat frame;
		capture >> frame;
		imshow("video", frame);

		int key = 0;
		static size_t i = 0;
		static size_t t = 0;

		if (t > 5){		// 隔一段时间再判断，否则会连续判断，导致多次截图
			key = GetKeyState(VK_SHIFT);	//获取shift键状态
			t = 0;
		}
		t++;

		if (key < 0){
			char fileName[50];
			sprintf_s(fileName, "E:\\Cplusplus_up\\VC0803\\screen_capture\\%d.png", i); //保存图片的路径
			imwrite(fileName, frame);
			cout << i << "截图成功" << endl;
			i++;
		}

		waitKey(30);	//延时30
	}

	return 0;
}

