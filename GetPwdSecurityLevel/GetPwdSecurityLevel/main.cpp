#include <iostream>
#include <string>

using namespace std;
class GetPwdSecurityLevel
{
public:
	GetPwdSecurityLevel(string password_str) :_score(0), _password_str(password_str){}
	int size_score(){
		if (_password_str.size() >= 0 && _password_str.size() <= 4){
			return 5;
		}
		else if (_password_str.size() >= 5 && _password_str.size() <= 7){
			return 10;
		}
		else if (_password_str.size() >= 8){
			return 25;
		}
	}
	int letter_score(){
		int a_letter_num = 0;
		int A_letter_num = 0;
		int number_num = 0;
		int symbol_num = 0;
		for (auto& e : _password_str){
			if (e >= 'a' && e <= 'z'){
				++a_letter_num;
			}
			else if (e >= 'A' && e <= 'Z'){
				++A_letter_num;
			}
			else if (e >= '0' && e <= '9'){
				++number_num;
			}
			else{
				++symbol_num;
			}
		}
		if ((a_letter_num == _password_str.size() || A_letter_num == _password_str.size())){

		}

	}
	string GetLevel(){
		if (_score >= 0 && _score < 25){
			return "VERY_WEAK";
		}
		else if (_score >= 25 && _score < 50){
			return "WEAK";
		}
		else if (_score >= 50 && _score < 60){
			return "AVERAGE";
		}
		else if (_score >= 60 && _score < 70){
			return "STRONG";
		}
		else if (_score >= 70 && _score < 80){
			return "VERY_STRONG";
		}
		else if (_score >= 80 && _score < 90){
			return "SECURE";
		}
		else if (_score >= 90){
			return "VERY_SECURE";
		}
	}

public:
	int _score;
	string _password_str;
};

int main(){
	string password_str;
	while (getline(cin, password_str)){
		GetPwdSecurityLevel GPL(password_str);
		cout << GPL.GetLevel() << endl;
	}
	system("pause");
	return 0;
}