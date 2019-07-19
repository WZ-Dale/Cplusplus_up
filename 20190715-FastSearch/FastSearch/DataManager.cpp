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
	assert(_db);	//���_dbΪ��˵�����ݿ�û�г�ʼ��
	char* errmsg;
	int ret = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errmsg);
	if (ret != SQLITE_OK){
		ERROE_LOG("sqlite3_exec(%s) errmsg:%s\n", sql.c_str(), errmsg);	//��ʾִ���ľ����ݿ�ʧ����
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
	std::unique_lock<std::mutex> lock(_mtx);
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
	std::unique_lock<std::mutex> lock(_mtx);
	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	lock.unlock();
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
	std::unique_lock<std::mutex> lock(_mtx);
	_dbmgr.ExecuteSql(sql);
}
void DataManager::DeleteDoc(const string& path, const string& name){
	// ɾ�����ļ�
	char sql[256];
	sprintf(sql, "delete from %s where path = '%s' and name = '%s'", TB_NAME, path.c_str(), name.c_str());
	_dbmgr.ExecuteSql(sql);
	// ��Ҫɾ���ļ�Ŀ¼�µ����ļ�
	string path_ = path + '\\' + name;
	sprintf(sql, "delete from %s where path like '%s%%'", TB_NAME, path_.c_str());
	std::unique_lock<std::mutex> lock(_mtx);
	_dbmgr.ExecuteSql(sql);
}
void DataManager::Search(const string& key, vector<std::pair<string, string>>& docinfos){
	//char sql[256];
	//sprintf(sql, "select name, path from %s where name like '%%%s%%'", TB_NAME, key.c_str());
	//int row, col;
	//char** ppRet;
	//AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	//for (int i = 1; i <= row; ++i){
	//	docinfos.push_back(std::make_pair(ppRet[i * col + 0], ppRet[i * col + 1]));
	//}
	char sql[256];
	{
		string pinyin = ChineseConvertPinYinAllSpell(key);	//�ѹؼ���תΪƴ��,ʹ��ƴ������
		string initials = ChineseConvertPinYinInitials(key);	//�ѹؼ���תΪƴ������ĸ,ʹ��ƴ������ĸ����
		sprintf(sql, "select name, path from %s where name_pinyin like '%%%s%%' or name_initials like '%%%s%%'", TB_NAME, pinyin.c_str(), initials.c_str());
		int row, col;
		char** ppRet;
		std::unique_lock<std::mutex> lock(_mtx);
		AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
		lock.unlock();
		for (int i = 1; i <= row; ++i){
			docinfos.push_back(std::make_pair(ppRet[i * col + 0], ppRet[i * col + 1]));
		}
	}
	//��Ӿֲ���
	//{
	//	string initials = ChineseConvertPinYinInitials(key);	//�ѹؼ���תΪƴ������ĸ,ʹ��ƴ������ĸ����
	//	sprintf(sql, "select name, path from %s where name_initials like '%%%s%%'", TB_NAME, initials.c_str());
	//	int row, col;
	//	char** ppRet;
	//	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	//	for (int i = 1; i <= row; ++i){
	//		docinfos.push_back(std::make_pair(ppRet[i * col + 0], ppRet[i * col + 1]));
	//	}
	//}
}
void DataManager::SplitHighlight(const string& name, const string& key, string& prefix, string& highlight, string& suffix){
	//1.key��ԭ�����Ӵ�
	{
		size_t ht_start = name.find(key);
		if (ht_start != string::npos){

			prefix = name.substr(0, ht_start);
			highlight = key;
			suffix = name.substr(ht_start + key.size(), string::npos);

			return;
		}		
	}
	//2.key��ƴ��ȫƴ,ƥ�人��
	{
		string key_ap = ChineseConvertPinYinAllSpell(key);	//keyתƴ��
		string name_ap = ChineseConvertPinYinAllSpell(name);	//ԭ��תƴ��
		size_t ap_start = name_ap.find(key_ap);	//�ҵ�ƴ��ƥ��λ��
		size_t ap_end = ap_start + key_ap.size();
		size_t ap_index = 0;	//ԭ��ת��ƴ���е�ָ��
		size_t ht_index = 0;	//ԭ�������е�ָ��
		size_t ht_start = 0, ht_len = 0;		//��������
		if (ap_start != string::npos){
			while (ap_index < ap_end){
				if (ap_index == ap_start){
					ht_start = ht_index;
				}
				if (name[ht_index] >= 0 && name[ht_index] < 127){	//�����ASCII�ַ�
					++ht_index;
					++ap_index;
				}
				else{
					char chinese[3] = { '\0' };
					chinese[0] = name[ht_index];
					chinese[1] = name[ht_index + 1];
					string py_str = ChineseConvertPinYinAllSpell(chinese);
					ap_index += py_str.size();	//�����ú���ƴ���ĳ���
					ht_index += 2;	//gbk���������ַ�
				}
			}
			ht_len = ht_index - ht_start;

			prefix = name.substr(0, ht_start);
			highlight = name.substr(ht_start, ht_len);
			suffix = name.substr(ht_start + ht_len, string::npos);

			return;
		}
	}
	//3.key������ĸ,ƥ�人��
	{
		string key_it = ChineseConvertPinYinInitials(key);	//keyת����ĸ
		string name_it = ChineseConvertPinYinInitials(name);	//ԭ��ת����ĸ
		size_t it_start = name_it.find(key_it);
		size_t it_end = it_start + key_it.size();
		size_t it_index = 0;	//ԭ��ת������ĸ�е�ָ��
		size_t ht_index = 0;	//ԭ�������е�ָ��
		size_t ht_start = 0, ht_len = 0;		//��������
		if (it_start != string::npos){
			while (it_index < it_end){
				if (it_index == it_start){
					ht_start = ht_index;
				}
				if (name[ht_index] >= 0 && name[ht_index] < 127){	//�����ASCII�ַ�
					++ht_index;
					++it_index;
				}
				else{
					++it_index;	//�����ú�������ĸ
					ht_index += 2;	//gbk���������ַ�
				}
			}
			ht_len = ht_index - ht_start;

			prefix = name.substr(0, ht_start);
			highlight = name.substr(ht_start, ht_len);
			suffix = name.substr(ht_start + ht_len, string::npos);

			return;
		}
	}
	TRACE_LOG("Split highlight no match. name: %s, key: %s\n", name.c_str(), key.c_str());
	prefix = name;
}

