#include "DataManager.h"

void SqliteManager::Open(const string& path){
	int ret = sqlite3_open(path.c_str(), &_db);
	if (ret != SQLITE_OK){
		ERROE_LOG("sqlite3_open\n");
	}
	else{
		TRACE_LOG("sqlite3_open success\n");
	}
}
void SqliteManager::Close(){
	int ret = sqlite3_close(_db);
	if (ret != SQLITE_OK){
		ERROE_LOG("sqlite3_close\n");
	}
	else{
		TRACE_LOG("sqlite3_close success\n");
	}
}
void SqliteManager::ExecuteSql(const string& sql){
	assert(_db);	//如果_db为空说明数据库没有初始化
	char* errmsg;
	int ret = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errmsg);
	if (ret != SQLITE_OK){
		ERROE_LOG("sqlite3_exec(%s) errmsg:%s\n", sql.c_str(), errmsg);	//显示执行哪句数据库失败了
		sqlite3_free(errmsg);
	}
	else{
		TRACE_LOG("sqlite3_exec(%s) success\n", sql.c_str());
	}
}
void SqliteManager::GetTable(const string& sql, int& row, int& col, char**& ppRet){
	char* errmsg;
	int ret = sqlite3_get_table(_db, sql.c_str(), &ppRet, &row, &col, &errmsg);
	if (ret != SQLITE_OK){
		ERROE_LOG("sqlite3_get_table(%s) errmsg:%s\n", sql.c_str(), errmsg);
		sqlite3_free(errmsg);
	}
	else{
		TRACE_LOG("sqlite3_get_table(%s) success\n", sql.c_str());
	}
}

void DataManager::Init(){
	_dbmgr.Open(DB_NAME);
	char sql[256];
	sprintf(sql, "create table if not exists %s(id integer primary key, path text, name text, name_pinyin text, name_initials text)", TB_NAME);
	//sprintf(sql, "create table if not exists %s(id integer primary key, path text, name text, allspell text, initials text)", TB_NAME);
	_dbmgr.ExecuteSql(sql);
}
void DataManager::GetDocs(const string& path, std::set<string>& dbset){
	char sql[256];
	sprintf(sql, "select name from %s where path = '%s'", TB_NAME, path.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i){
		for (int j = 0; j < col; ++j){
			dbset.insert(ppRet[i * col + j]);
		}
	}
}
void DataManager::InsertDoc(const string& path, const string& name){
	char sql[256];
	string allspell = ChineseConvertPinYinAllSpell(name);
	string initials = ChineseConvertPinYinInitials(name);
	sprintf(sql, "insert into %s(path, name, name_pinyin, name_initials) values('%s', '%s', '%s', '%s')", TB_NAME, path.c_str(), name.c_str(), allspell.c_str(), initials.c_str());
	//sprintf(sql, "insert into %s(path, name, allspell, initials) values('%s', '%s', '%s', '%s')", TB_NAME, path.c_str(), name.c_str(), allspell.c_str(), initials.c_str());
	_dbmgr.ExecuteSql(sql);
}
void DataManager::DeleteDoc(const string& path, const string& name){
	// 删除本文件
	char sql[256];
	sprintf(sql, "delete from %s where path = '%s' and name = '%s'", TB_NAME, path.c_str(), name.c_str());
	_dbmgr.ExecuteSql(sql);
	// 还要删该文件目录下的子文件
	string path_ = path + '\\' + name;
	sprintf(sql, "delete from %s where path like '%s%%'", TB_NAME, path_.c_str());
	_dbmgr.ExecuteSql(sql);
}
void DataManager::Search(const string& key, vector<std::pair<string, string>>& docinfos){
	char sql[256];
	sprintf(sql, "select name, path from %s where name like '%%%s%%'", TB_NAME, key.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i){
		docinfos.push_back(std::make_pair(ppRet[i * col + 0], ppRet[i * col + 1]));
	}
}

