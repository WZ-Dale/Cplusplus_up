#pragma once

#include "Common.h"

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
class DataManager
{
public:
	void Init();	//建表,打开数据库
	void GetDocs(const string& path, std::set<string>& dbset);	//查找path下的所有子文档
	void InsertDoc(const string& path, const string& name);	//插入
	void DeleteDoc(const string& path, const string& name);	//删除
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);	//搜索
private:
	SqliteManager _dbmgr;
};
