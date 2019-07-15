#pragma once

#include "Common.h"

class ScanManager	//扫描出信息,放到数据库中
{
public:
	void Scan(const string& path){
		//比对文件系统和数据库
		vector<string>	localdirs, localfiles;
		DirectoryList(path, localdirs, localfiles);
		std::set<string> localset;
		localset.insert(localdirs.begin(), localdirs.end());
		localset.insert(localfiles.begin(), localfiles.end());
		vector<string>	dbdirs, dbfiles;



	}
private:

};
