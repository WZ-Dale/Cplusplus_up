#include "Common.h"
#include "DataManager.h"
#include "ScanManager.h"

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
void TestScanManager(){
	//ScanManager scanmgr;
	//scanmgr.Scan("E:\\Cplusplus_up\\20190715-FastSearch1\\FastSearch");
}
void TestSearch(){
	//ScanManager scanmgr;
	//scanmgr.Scan("E:\\Cplusplus_up\\20190715-FastSearch1\\FastSearch");
	ScanManager::CreateInstance();

	DataManager datamgr;
	datamgr.Init();	//记得初始化
	vector<std::pair<string, string>> docinfos;
	string key;
	cout << "==============================================================================" << endl;
	cout << "=========================== 快 --- 速 --- 搜 --- 索 ==========================" << endl;
	cout << "==============================================================================" << endl;
	cout << "请输入要搜索的关键字: ";
	while (cin >> key){
		docinfos.clear();
		datamgr.Search(key, docinfos);
		if (docinfos.empty()){
			cout << "\n=============================>> 未找到相关文件 <<=============================" << endl;
		}
		else{
			printf("\n%-30s%-50s\n", "文件名", "路径");
			for (const auto& e : docinfos){
				//cout << e.first << "   " << e.second << endl;
				const string& name = e.first;
				const string& path = e.second;
				string prefix, highlight, suffix;
				datamgr.SplitHighlight(name, key, prefix, highlight, suffix);
				cout << prefix;
				ColourPrintf(highlight.c_str());
				cout << suffix;
				for (size_t i = name.size(); i < 30; ++i){	//补齐空格
					cout << " ";
				}
				printf("%-50s\n", path.c_str());
			}
		}
		cout << "\n==============================================================================" << endl;
		cout << "请输入要搜索的关键字: ";
	}
}
void TestPinyin(){
	string allspell = ChineseConvertPinYinAllSpell("新建  王泽   wangze  zegg来一打");
	string initials = ChineseConvertPinYinInitials("新建  哥哥   gegege  zegg来一打");
	cout << allspell << endl;
	cout << initials << endl;
}
void TestHighlight(){
	//ColourPrintf("王泽wangze新建字符串Dale\n");
	//1.直接高亮汉字key
	{
		string key = "新建";
		string str = "王泽wangze新建字符串gege";
		size_t pos = str.find(key);
		string prefix, suffix;
		prefix = str.substr(0, pos);
		suffix = str.substr(pos + key.size(), string::npos);
		cout << prefix;
		ColourPrintf(key.c_str());
		cout << suffix << endl;
	}
	//2.key是拼音,高亮汉字
	{
		string key = "xinjian";
		string str = "abc王泽wangze新建字符串gege";
		string prefix, suffix;
		string key_py = ChineseConvertPinYinAllSpell(key);	//key转拼音
		string str_py = ChineseConvertPinYinAllSpell(str);	//原串转拼音
		size_t pos = str_py.find(key_py);	//找到转成拼音后的匹配位置
		if (pos == string::npos){
			cout << "拼音不匹配" << endl;
		}
		else{
			size_t key_start = pos;
			size_t key_end = pos + key_py.size();

			size_t str_i = 0;	//原串中的指针
			size_t py_i = 0;	//原串转成拼音中的指针
			while (py_i < key_start){
				if (str[str_i] >=0 && str[str_i] < 127){	//如果是ASCII字符
					++str_i;
					++py_i;
				}
				else{
					char chinese[3] = { '\0' };
					chinese[0] = str[str_i];
					chinese[1] = str[str_i + 1];
					string py_str = ChineseConvertPinYinAllSpell(chinese);
					py_i += py_str.size();
					str_i += 2;
				}
			}	//当py_i走到pos的时候,str_i就走到了对应匹配汉字的位置
			prefix = str.substr(0, str_i);

			size_t str_j = str_i;
			size_t py_j = py_i;
			while (py_j < key_end){
				if (str[str_j] >= 0 && str[str_j] < 127){	//如果是ASCII字符
					++str_j;
					++py_j;
				}
				else{
					char chinese[3] = { '\0' };
					chinese[0] = str[str_j];
					chinese[1] = str[str_j + 1];
					string py_str = ChineseConvertPinYinAllSpell(chinese);
					py_j += py_str.size();
					str_j += 2;
				}
			}
			key = str.substr(str_i, str_j - str_i);	//不要忘了第二个参数传的是长度

			suffix = str.substr(str_j, string::npos);

			cout << prefix;
			ColourPrintf(key.c_str());
			cout << suffix << endl;
		}
	}
	//3.key是首字母,高亮对应的汉字
	{
		string key = "xj";
		string str = "王泽wangze新建字符串gege";
		string prefix, suffix;
		string initials = ChineseConvertPinYinInitials(str);

	}
}


int main(){
	//TestDirectoryList();
	//TestSqlite();
	//TestScanManager();
	TestSearch();
	//TestPinyin();
	//TestHighlight();
	system("pause");
	return 0;
}

