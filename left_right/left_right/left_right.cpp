#include <iostream>
#include <math.h>
#include <iomanip>

int main(){
	while (1){
		double a = 0.0, b = 0.0;
		std::cout << "������һ��������: ";
		std::cin >> a;
		std::cout << "������һ������ƫ��: ";
		std::cin >> b;
		std::cout << std::fixed << std::setprecision(5) << a - b << "  --  " << a + b << "\n" << std::endl;
	}
	system("pause");
	return 0;
}
