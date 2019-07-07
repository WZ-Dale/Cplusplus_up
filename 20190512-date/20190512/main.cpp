#include <iostream>
#include <stdlib.h>
#include "date.h"

void Test(){
	Date d1;
	d1.Print();//1996-6-23
	Date d2(1995, 4, 29);
	d2.Print();//1995-4-29
	cout << (d1 > d2) << endl;//1
	cout << (d1 >= d2) << endl;//1
	cout << (d1 < d2) << endl;//0
	cout << (d1 <= d2) << endl;//0
	cout << (d1 == d2) << endl;//0
	cout << (d1 != d2) << endl;//1
	++d1;
	d1.Print();//1996-6-24
	d1++;
	d1.Print();//1996-6-25
	--d1;
	d1.Print();//1996-6-24
	d1--;
	d1.Print();//1996-6-23
	Date d3 = d1 + 10;
	d3.Print();//1996-7-3
	d1 += 10;
	d1.Print();//1996-7-3
	Date d4 = d1 - 10;
	d4.Print();//1996-6-23
	d1 -= 10;
	d1.Print();//1996-6-23
	cout << (d1 - d2) << endl;//421
}

int main(){
	Test();
	system("pause");
	return 0;
}
