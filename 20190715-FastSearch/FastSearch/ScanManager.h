#pragma once

#include "Common.h"
#include "DataManager.h"

class ScanManager	//扫描出信息,放到数据库中
{
public:
	ScanManager(){
		_datamgr.Init();
	}
	void Scan(const string& path);
private:
	DataManager _datamgr;
};
