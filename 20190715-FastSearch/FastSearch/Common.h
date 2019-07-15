#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files){	//����Ŀ¼�µ������ļ�
	_finddata_t file;
	const string path_ = path + "\\*.*";
	intptr_t handle = _findfirst(path_.c_str(), &file); //�õ����ڵ��µĵ�һ��������Ϣ,�ŵ�file��
	if (handle == -1){
		cout << "_findfirst: " << path << "error!" << endl;
		return;
	}
	do{	//�ݹ�ͬһ����������ļ�,ʹ��next
		if (file.attrib & _A_SUBDIR){	//���Ϊ��,��˵������ΪĿ¼
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)){
				dirs.push_back(file.name);	//��Ŀ¼�����ƴ���dirs��
			}			
		}
		else{	//����,˵������Ϊ�ļ�
			files.push_back(file.name);	//���ļ������ƴ���dirs��
		}
	} while (_findnext(handle, &file) == 0);	//����0˵����next,����


}

