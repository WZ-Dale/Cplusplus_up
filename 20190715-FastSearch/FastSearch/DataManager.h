#pragma once

#include "Common.h"

//对sqlite操作的封装
class SqliteManager
{
public:
	SqliteManager()
		:_db(nullptr)
	{}
	~SqliteManager(){
		Close();
	}
	void Open(const string& path);
	void Close();
	void ExecuteSql(const string& sql);
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);

	SqliteManager(const SqliteManager&) = delete;
	SqliteManager& operator=(const SqliteManager&) = delete;
private:
	sqlite3* _db; // 数据库对象
};

//RAII
class AutoGetTable
{
public:
	AutoGetTable(SqliteManager& sm, const string& sql, int& row, int& col, char**& ppRet){
		sm.GetTable(sql, row, col, ppRet);
		_ppRet = ppRet;
	}
	~AutoGetTable(){
		sqlite3_free_table(_ppRet);
	}
	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable operator=(const AutoGetTable&) = delete;
private:
	char** _ppRet;
};

//数据管理模块
#define DB_NAME "doc.db"
#define TB_NAME "tb_doc"
//为了方便加锁,设计成单例模式
//一般使用饿汉模式,不用考虑线程安全问题哦
class DataManager
{
public:
	static DataManager* GetInstance(){
		static DataManager datamgr;
		datamgr.Init();
		return &datamgr;
	}

	void Init();	//建表,打开数据库
	void GetDocs(const string& path, std::set<string>& dbset);	//查找path下的所有子文档
	void InsertDoc(const string& path, const string& name);	//插入
	void DeleteDoc(const string& path, const string& name);	//删除
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);	//搜索
	void SplitHighlight(const string& name, const string& key, string& prefix, string& highlight, string& suffix);	//分割高亮部分
private:
	DataManager(){}
	SqliteManager _dbmgr;
	std::mutex _mtx;
};

