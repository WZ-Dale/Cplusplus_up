#include "Common.h"

void TestDirectoryList(){
	vector<string>	dirs, files;
	DirectoryList("C:\\Users\\Administrator\\Documents\\Visual Studio 2013\\Projects\\Test", dirs, files);
	for (const auto& e : dirs){
		cout << e << endl;
	}
	for (const auto& e : files){
		cout << e << endl;
	}
}


int main(){
	TestDirectoryList();
	system("pause");
	return 0;
}
