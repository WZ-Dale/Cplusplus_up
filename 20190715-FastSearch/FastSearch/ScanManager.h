#pragma once

#include "Common.h"
#include "DataManager.h"

//��������ֻ��һ��ɨ��ģ��,��Ƴɵ���
class ScanManager	//ɨ�����Ϣ,�ŵ����ݿ���
{
public:
	void Scan(const string& path);
	void StartScan(){
		while (1){
			Scan("E:\\Cplusplus_up\\20190715-FastSearch1");
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	static ScanManager* CreateInstance(){
		static ScanManager scanmgr;
		static std::thread thd(&StartScan, &scanmgr);
		thd.detach();
		return &scanmgr;
	}
private:
	ScanManager(){
		_datamgr.Init();
	}
	ScanManager(const ScanManager&);
	DataManager _datamgr;
};

