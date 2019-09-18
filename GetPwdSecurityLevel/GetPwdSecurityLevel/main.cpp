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
		//ͳ�Ƴ��ִ���
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
		//����һ
		if (_size > 0 && _size <= 4){
			_score += 5;
		}
		else if (_size >= 5 && _size <= 7){
			_score += 10;
		}
		else if (_size >= 8){
			_score += 25;
		}
		//�����
		if ((a_letter_num == _size || A_letter_num == _size)){//ȫ����С����д��ĸ
			_score += 10;
		}
		else if (a_letter_num > 0 && A_letter_num > 0){//��Сд�����ĸ
			_score += 20;
		}
		//������
		if (number_num == 1){//1������
			_score += 10;
		}
		else if (number_num > 1){//����1������
			_score += 20;
		}
		//������
		if (symbol_num == 1){//1������
			_score += 10;
		}
		else if (symbol_num > 1){//����1������
			_score += 25;
		}
		//������
		if (a_letter_num + A_letter_num + number_num == _size){//��ĸ������
			_score += 2;
		}
		else if (a_letter_num + A_letter_num + number_num + symbol_num == _size){//��ĸ�����ֺͷ���
			_score += 3;
		}
		else if (a_letter_num > 0 && A_letter_num > 0 && a_letter_num + A_letter_num + number_num + symbol_num == _size){//��Сд��ĸ�����ֺͷ���
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