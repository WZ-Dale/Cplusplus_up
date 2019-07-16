#include "ScanManager.h"

void ScanManager::Scan(const string& path){
	//比对文件系统和数据库
	vector<string>	localdirs, localfiles;
	DirectoryList(path, localdirs, localfiles);

	std::set<string> localset;
	localset.insert(localdirs.begin(), localdirs.end());
	localset.insert(localfiles.begin(), localfiles.end());

	std::set<string> dbset;
	//DataManager()
	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end()){
		if (*localit < *dbit){	//本地有,数据库没有->给数据库新增
			//新增
			++localit;
		}
		else if (*localit > *dbit){	//本地没有,数据库有->删除数据库中的
			//删除
			++dbit;
		}
		else{	//没有变化的数据
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end()){	//本地数据没有走完
		//新增
		++localit;
	}
	while (dbit != dbset.end()){	//数据库数据没有走完
		//删除
		++dbit;
	}
	for (const auto& subdirs : localdirs){	//递归扫描比对子目录中的数据
		//Scan(subdirs);
	}
}

