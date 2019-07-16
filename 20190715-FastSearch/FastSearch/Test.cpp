#include "Common.h"
#include "ScanManager.h"
#include "DataManager.h"

void TestDirectoryList(){
	vector<string>	dirs, files;
	DirectoryList("E:\\Cplusplus_up\\20190715-FastSearch", dirs, files);
	for (const auto& e : dirs){
		cout << e << endl;
	}
	for (const auto& e : files){
		cout << e << endl;
	}
}
void TestSqlite(){
	SqliteManager sq;
	sq.Open("E:\\Cplusplus_up\\20190715-FastSearch\\FastSearch\\test.db");

	string createtb_sql = "create table tb_doc(id integer primary key autoincrement, doc_path text, doc_name text)";
	sq.ExecuteSql(createtb_sql);

	string insert_sql = "insert into tb_doc(doc_path, doc_name) values('E:\\Cplusplus_up\\20190715-FastSearch\\FastSearch\\test', 'test1.txt')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('E:\\Cplusplus_up\\20190715-FastSearch\\FastSearch\\test', 'test2.txt')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('E:\\Cplusplus_up\\20190715-FastSearch\\FastSearch\\test', 'test3.txt')";
	sq.ExecuteSql(insert_sql);

	string query_sql = "select * from tb_doc where doc_path = 'E:\\Cplusplus_up\\20190715-FastSearch\\FastSearch\\test'";
	int row, col;
	char** ppRet;
	/*
	sq.GetTable(query_sql, row, col, ppRet);
	for (int i = 1; i < row; ++i){
		for (int j = 0; j < col; ++j){
			cout << ppRet[i * col + j] << " ";
		}
		cout << endl;
	}
	sqlite3_free_table(ppRet);
	*/
	AutoGetTable agt(sq, query_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i){
		for (int j = 0; j < col; ++j){
			cout << ppRet[i * col + j] << " ";
		}
		cout << endl;
	}
}

int main(){
	//TestDirectoryList();
	TestSqlite();
	system("pause");
	return 0;
}
