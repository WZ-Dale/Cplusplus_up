#pragma once

#include "Common.h"

class ScanManager	//ɨ�����Ϣ,�ŵ����ݿ���
{
public:
	void Scan(const string& path){
		//�ȶ��ļ�ϵͳ�����ݿ�
		vector<string>	localdirs, localfiles;
		DirectoryList(path, localdirs, localfiles);
		std::set<string> localset;
		localset.insert(localdirs.begin(), localdirs.end());
		localset.insert(localfiles.begin(), localfiles.end());
		vector<string>	dbdirs, dbfiles;



	}
private:

};
