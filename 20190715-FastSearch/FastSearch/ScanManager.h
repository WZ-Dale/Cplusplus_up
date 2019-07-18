#pragma once

#include "Common.h"
#include "DataManager.h"

//整个程序只有一个扫描模块,设计成单例
class ScanManager	//扫描出信息,放到数据库中
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

