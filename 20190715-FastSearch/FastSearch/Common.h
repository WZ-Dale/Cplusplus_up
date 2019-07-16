#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

#include "./sqlite-amalgamation-3280000/sqlite3.h"

#define __DEBUG__
#define __TRACE__

static void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files){	//����Ŀ¼�µ������ļ�
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

// 
static std::string GetFileName(const std::string& path)
{
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
//���ڵ���׷�ݵ�trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const	char* format, ...)
{
#ifdef __TRACE__
	//������ú�������Ϣ
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, "\n");
#endif
}

inline static void __ErrorDebug(const char* filename, int line, const char* function, const	char* format, ...)
{
#ifdef __DEBUG__
	//������ú�������Ϣ
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, "errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}

#define TRACE_LOG(...) \
 __TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROE_LOG(...) \
 __ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);


