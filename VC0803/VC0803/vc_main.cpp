#include <iostream>
#include <windows.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	// ��ȡ��Ƶ������ͷ
	//VideoCapture capture("��Ƶ·��\\�ļ���.mp4");
	// ��ȡ����ͷ
	//VideoCapture capture0(0);	// �˴�0��PC������ͷ
	VideoCapture capture(1);	// �˴�1����ӵ�����ͷ

	// ����Ƿ�������:�ɹ���ʱ��isOpened����ture
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

		if (t > 5){		// ��һ��ʱ�����жϣ�����������жϣ����¶�ν�ͼ
			key = GetKeyState(VK_SHIFT);	//��ȡshift��״̬
			t = 0;
		}
		t++;

		if (key < 0){
			char fileName[50];
			sprintf_s(fileName, "E:\\Cplusplus_up\\VC0803\\screen_capture\\%d.png", i); //����ͼƬ��·��
			imwrite(fileName, frame);
			cout << i << "��ͼ�ɹ�" << endl;
			i++;
		}

		waitKey(30);	//��ʱ30
	}

	return 0;
}

