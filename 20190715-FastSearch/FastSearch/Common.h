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

static void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files){	//遍历目录下的所有文件
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
//用于调试追溯的trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const	char* format, ...)
{
#ifdef __TRACE__
	//输出调用函数的信息
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
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
	//输出调用函数的信息
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
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


