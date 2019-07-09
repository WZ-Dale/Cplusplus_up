#include "date.h"
// 该代码主要以>,==,+=,-=为基础写的
bool Date::operator>(const Date& d) const{
	if (_year > d._year){
		return true;
	}
	else if (_year == d._year){
		if (_month > d._month){
			return true;
		}
		else if (_month == d._month){
			if (_day > d._day){
				return true;
			}
		}
	}
	return false;
}
bool Date::operator>=(const Date& d) const{
	return (*this > d || *this == d);
}
bool Date::operator<(const Date& d) const{
	return !(*this >= d);
}
bool Date::operator<=(const Date& d) const{
	return !(*this > d);
}
bool Date::operator==(const Date& d) const{
	return (_year == d._year && _month == d._month && _day == d._day);
}
bool Date::operator!=(const Date& d) const{
	return !(*this == d);
}

Date& Date::operator++(){ //前置++
	*this += 1;
	return *this;
}
Date Date::operator++(int){ //后置++
	Date ret(*this);
	*this += 1;
	return ret;
}
Date Date::operator+(int day) const{
	Date ret = *this;
	ret += day;
	return ret;
}
Date& Date::operator+=(int day){
	_day += day;
	while (_day > GetMonthDay(_year, _month)){ //如果加了之后天数大于本月天数则进入
		_day -= GetMonthDay(_year, _month);
		++_month;
		if (_month == 13){
			_month = 1;
			++_year;
		}
	}
	return *this;
}

Date& Date::operator--(){ //前置--
	*this -= 1;
	return *this;
}
Date Date::operator--(int){ //后置--
	Date ret(*this);
	*this -= 1;
	return ret;
}
//Date Date::operator-(int day) const{
//	Date ret = *this;
//	ret -= day;
//	return ret;
//}
Date Date::operator-(int day){
	Date ret = *this;
	ret -= day;
	return ret;
}
Date& Date::operator-=(int day){
	if (day <= 0){ //若要减的是一个负数则相当于加-day
		return *this += -day;
	}
	_day -= day;
	while (_day < 1){ //如果减了之后天数小于1则进入
		--_month;
		if (_month == 0){
			_month = 12;
			--_year;
		}
		_day += GetMonthDay(_year, _month);
	}
	return *this;
}
int Date::operator-(const Date& d){
	int ret = 0;
	if (*this < d){
		while (*this != d){
			++*this;
			++ret;
		}
	}
	else if (*this > d){
		while (*this != d){
			--*this;
			++ret;
		}
	}
	return ret;
}

