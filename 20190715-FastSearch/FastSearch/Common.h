#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files){	//遍历目录下的所有文件
	_finddata_t file;
	const string path_ = path + "\\*.*";
	intptr_t handle = _findfirst(path_.c_str(), &file); //拿到根节点下的第一个孩子信息,放到file里
	if (handle == -1){
		cout << "_findfirst: " << path << "error!" << endl;
		return;
	}
	do{	//递归同一级层的所有文件,使用next
		if (file.attrib & _A_SUBDIR){	//如果为真,则说明属性为目录
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)){
				dirs.push_back(file.name);	//将目录的名称存入dirs中
			}			
		}
		else{	//否则,说明属性为文件
			files.push_back(file.name);	//将文件的名称存入dirs中
		}
	} while (_findnext(handle, &file) == 0);	//等于0说明有next,继续


}

