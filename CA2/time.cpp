#include "CA2.h"
#include <iostream>

Time::Time()
{
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    second = 0;
}

Time* Time::operator+(Time *sec)
{
    Time* result = new Time();

    if((this->second + sec->second + result->second) >= 60)
    {
        result->second = (this->second + sec->second) - 60;
        result->minute++;
    }
    else result->second += (this->second + sec->second);

    if((this->minute + sec->minute + result->minute) >= 60)
    {
        result->minute = (this->minute + sec->minute + result->minute) - 60;
        result->hour++;
    }
    else result->minute += (this->minute + sec->minute);

    if((this->hour + sec->hour + result->hour) >= 24)
    {
        result->hour = (this->hour + sec->hour + result->hour) - 24;
        result->day++;
    }
    else result->hour += (this->hour + sec->hour);

    if((this->day + sec->day + result->day) >= 31)
    {
        result->day = (this->day + sec->day + result->day) - 30;
        result->month++;
    }
    else result->day += (this->day + sec->day);  

    if((this->month + sec->month + result->month) >= 13)
    {
        result->month = (this->month + sec->month + result->month) - 12;
        result->year++;
    }
    else result->month += (this->month + sec->month); 

    result->year += (this->year + sec->year);

    return result;
}



Time* Time::operator-(Time *sec)    
{
    Time first = *this;
    Time second = *sec;
    Time* result = new Time();
    if((this->operator>(sec))==false){
        cout << "Error! Time subtraction goes wrong!" <<endl;
        exit(1);
    }

    if((first.second - second.second) < 0)
    {
        result->second = (first.second - second.second) + 60;
        first.minute--;
    }
    else result->second += (first.second - second.second);

    if((first.minute - second.minute) < 0)
    {
        result->minute = (first.minute - second.minute) + 60;
        first.hour--;
    }
    else result->minute += (first.minute - second.minute);

    if((first.hour - second.hour) < 0)
    {
        result->hour = (first.hour - second.hour) + 24;
        first.day--;
    }
    else result->hour += (first.hour - second.hour);

    if((first.day - second.day) <= 0)           //something with problem
    {
        result->day = (first.day - second.day) + 30;
        first.month--;
    }
    else result->day += (first.day - second.day);  

    if((first.month - second.month) <= 0)
    {
        result->month = (first.month - second.month) + 12;
        first.year--;
    }
    else result->month += (first.month - second.month); 

    result->year += (first.year - second.year);

    return result;
}


Time* subtract_time(Time *sec1, Time * sec2){       
    // cout << sec1 << "  " << sec2 << endl;
    int sec_this = 0;
    int sec_para = 0;
    int sec_diff, diff_year, diff_month, diff_day, diff_hour, diff_minute, diff_second;
    // cout<< "sec1->year "<< sec1->year << " sec1->month " << sec1->month << " sec1->day " << sec1->day << " sec1->hour " << sec1->hour<<" sec1->minute " << sec1->minute<<" sec1->second " << sec1->second<<endl;
    // cout<< "sec2->year "<< sec2->year << " sec2->month " << sec2->month << " sec2->day " << sec2->day << " sec2->hour " << sec2->hour<<" sec2->minute " << sec2->minute<<" sec2->second " << sec2->second<<endl;
    sec_this += (sec1->year-2020) * 31104000  + 2592000 * sec1->month + 86400 * sec1->day + 3600 * sec1->hour + 60 * sec1->minute + sec1->second;
    sec_para += (sec2->year-2020) * 31104000 + 2592000 * sec2->month + 86400 * sec2->day + 3600 * sec2->hour + 60 * sec2->minute + sec2->second;
    // cout<< "total seconds from sec1 "<< sec_this<< endl;
    // cout<< "total seconds from sec2 "<< sec_para<< endl;
    sec_diff = sec_this-sec_para;
    // cout<< "sec_diff "<< sec_diff<< endl;
    if(sec_diff < 0){
        cout << "sec1: " << sec1->year<< " " << sec1->month << " " << sec1->day << " " << sec1->hour<<endl;
        cout << "sec2: " << sec2->year<< " " << sec2->month << " " << sec2->day << " " << sec2->hour<<endl;
        cout << "Error! Time subtraction goes wrong!" <<endl;
        exit(1);
    }

    diff_year = sec_diff/31104000;      //should plus 2020
    diff_month = (sec_diff % 31104000) / 2592000;
    diff_day = (sec_diff % 31104000 % 2592000) / 86400;
    diff_hour = (sec_diff % 31104000 % 2592000 % 86400) / 3600;
    diff_minute = (sec_diff % 31104000 % 2592000 % 86400 % 3600) / 60;
    diff_second = sec_diff % 31104000 % 2592000 % 86400 % 3600 % 60;
    
    Time* result = new Time();
    result->year = diff_year;
    result->month = diff_month;
    result->day = diff_day;
    result->hour = diff_hour;
    result->minute = diff_minute;
    result->second = diff_second;

    return result;
}

int sec_time_upper_bound(int num){
    Time *conversion = new Time();
    int sec_total = 3 * 12 * 30 * 24 * 60 * 60;         //total second from 2019.1.1 to 2022.1.1
    int sec_upper_bound = sec_total / num;
    return sec_upper_bound;
}

Time* pointer_time_upper_bound(int sec_upper_bound){
    Time *conversion = new Time();
    conversion->year = sec_upper_bound / 31104000;
    conversion->month = (sec_upper_bound % 31104000) / 2592000;
    conversion->day = (sec_upper_bound % 31104000 % 2592000) / 86400;
    conversion->hour = (sec_upper_bound % 31104000 % 2592000 % 86400) / 3600;
    conversion->minute = (sec_upper_bound % 31104000 % 2592000 % 86400 % 3600) / 60;
    conversion->second = sec_upper_bound % 31104000 % 2592000 % 86400 % 3600 % 60;
    return conversion;
}

bool Time::operator>(Time *sec)
{
    if(this->year > sec->year) return true;
    else if(this->year == sec->year){
        if(this->month > sec->month) return true;
        else if(this->month == sec->month){
            if(this->day > sec->day) return true;
            else if(this->day == sec->day){
                if(this->hour > sec->hour) return true;
                else if(this->hour == sec->hour){
                    if(this->minute > sec->minute) return true;
                    else if(this->minute == sec->minute){
                        if(this->second > sec->second) return true;
                    }
                }
            }
        }
    }
    
    return false;
}

bool Time::operator<=(Time *sec)
{
    if(this->year > sec->year) return false;
    else if(this->year == sec->year){
        if(this->month > sec->month) return false;
        else if(this->month == sec->month){
            if(this->day > sec->day) return false;
            else if(this->day == sec->day){
                if(this->hour > sec->hour) return false;
                else if(this->hour == sec->hour){
                    if(this->minute > sec->minute) return false;
                    else if(this->minute == sec->minute){
                        if(this->second > sec->second) return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Time::operator==(Time *sec)
{
    if((this->year == sec->year) && (this->month == sec->month) && (this->day == sec->day)
        && (this->hour == sec->hour) && (this->minute == sec->minute) && (this->second == sec->second))
        {   return true;}
    else return false;
}

int convert_difference_to_days(Time* a, Time* b){
    Time* difference = new Time();
    // cout<<"debug_convert_difference_to_days calls subtract_time"<<endl;
    difference = subtract_time(a, b);
    int days=0;
    //cout << "difference->year:" << difference->year << "difference->month:" << difference->month << "difference->day:" << difference->day <<endl;
    days += 360*(difference->year) + 30*(difference->month) + difference->day;  //because we assume that each month has equally thirty days
    return days;
}

