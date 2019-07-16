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
	sprintf(sql, "create table if not exists %s(id integer primary key, path text, name text, name_pinyin text, name_initials text", TB_NAME);
	_dbmgr.ExecuteSql(sql);
}
void DataManager::GetDocs(const string& path){

}
void DataManager::InsertDoc(const string path, string name){
	char sql[256];
	sprintf(sql, "insert into %s(path, name) values('%s', '%s')", TB_NAME, path.c_str(), name.c_str());
	_dbmgr.ExecuteSql(sql);
}
void DataManager::DeleteDoc(const string path, string name){

}