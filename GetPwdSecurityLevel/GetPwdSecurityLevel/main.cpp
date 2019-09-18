#include <iostream>
#include <string>

using namespace std;

class GetPwdSecurityLevel
{
public:
	GetPwdSecurityLevel(string password_str) 
		:_password_str(password_str), _size(password_str.size()), _score(0) {}
	
	void statistics(){
		int a_letter_num = 0, A_letter_num = 0;
		int number_num = 0;
		int symbol_num = 0;
		//统计出现次数
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
			else {
				++symbol_num;
			}
		}
		//规则一
		if (_size > 0 && _size <= 4){
			_score += 5;
		}
		else if (_size >= 5 && _size <= 7){
			_score += 10;
		}
		else if (_size >= 8){
			_score += 25;
		}
		//规则二
		if ((a_letter_num == _size || A_letter_num == _size)){//全都是小（大）写字母
			_score += 10;
		}
		else if (a_letter_num > 0 && A_letter_num > 0){//大小写混合字母
			_score += 20;
		}
		//规则三
		if (number_num == 1){//1个数字
			_score += 10;
		}
		else if (number_num > 1){//大于1个数字
			_score += 20;
		}
		//规则四
		if (symbol_num == 1){//1个符号
			_score += 10;
		}
		else if (symbol_num > 1){//大于1个符号
			_score += 25;
		}
		//规则五
		if (a_letter_num + A_letter_num + number_num == _size){//字母和数字
			_score += 2;
		}
		else if (a_letter_num + A_letter_num + number_num + symbol_num == _size){//字母、数字和符号
			_score += 3;
		}
		else if (a_letter_num > 0 && A_letter_num > 0 && a_letter_num + A_letter_num + number_num + symbol_num == _size){//大小写字母、数字和符号
			_score += 5;
		}
	}
	string GetLevel(){
		statistics();
		if (_size == 0 || _score >= 0 && _score < 25){
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
	string _password_str;
	int _size;
	int _score;
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