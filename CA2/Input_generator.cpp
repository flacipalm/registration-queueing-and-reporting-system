#include <iostream>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <math.h>
#include "CA1.h"

using namespace std;


void input_generator(int local_order, int num, Hashset* database){
    /*  
        "local_order" indicate the order of the according local queue, (0~4). 
        "num" is the number of input people, and in they are put in an input_file in time order.
        Use "database" to check if every id is unique.
    */

    srand((int)time(NULL)); // Different seed for every execution.
    
    ofstream fout;
    string filename;
    filename = "./My_example/New_examplefile" + to_string(local_order+1) + ".txt";
    fout.open(filename); // It will cover last report, if there is any.
    fout << num << endl << endl; // First line of the input_file.

    int id; // Seven digits.
    string name;
    int loc1, loc2; // Coordinates.
    int phone_number; // nine digits.
    string wechat;  //"wxid" + five digits
    string email;   //name.age@intl.zju.edu.cn
    int pro_cat;    //0~8
    int birth_year, birth_month, birth_day;
    int age;
    int risk; // 0~3.
    int reg_year, reg_month, reg_day, reg_hour, reg_min, reg_sec;

    vector<int> id_set; // Store the id in this input_file.
	Time* last_reg = new Time; // Store the regist time of last person
	last_reg->year = 2019;
	last_reg->month = 1;
	last_reg->day = 1;
	last_reg->hour = 0;
	last_reg->minute = 0;
	last_reg->second = 0;

    // last_reg->year = 0;
	// last_reg->month = 1;
	// last_reg->day = 1;
	// last_reg->hour = 0;
	// last_reg->minute = 0;
	// last_reg->second = 0;


    

    for (int i = 0; i < num; i++){
        

        /**********************************/
		vector<int>::iterator result;
        int try_time = 0;
        do {
            try_time++;
            if (try_time > 1000){
				fout.close();
                cout << "To much time for setting date!" << endl;
                exit(EXIT_FAILURE);
			}
			id = 1000000; // It should be unique.
            for (int i = 0; i <= 5; i++ ) {
                id += (rand() % 10) * ((int) pow(10,i));
            } 
            result = find(id_set.begin(), id_set.end(), id);
            if(result == id_set.end() && database->member(id) == nullptr){
				id_set.push_back(id); 
				break;
			} 
        } while(result != id_set.end() || database->member(id) != NULL);
        /**********************************/

        name = "Robot" + to_string(rand() % 1000);
        loc1 = rand() % 10000;
        loc2 = rand() % 10000;
		phone_number = 100000000;
        for (int i = 0; i <= 7; i++ ) phone_number += (rand() % 10) * ((int) pow(10,i));
        wechat = "wxid-" + to_string(rand()%100000);
        pro_cat = rand() % 8;
        birth_year = 1920 + rand() % 100;
        birth_month = 1 + rand() % 12;
        birth_day = 1 + rand() % 30;
        risk = rand() % 4;
        age = 2021 - birth_year;
        email = name + "."+ to_string(age) + "@intl.zju.edu.cn";
        /**********************************/
		//int try_time = 0; // Record the time of generating the regist time for a person.
        //Time* cur_reg = new Time; // Current regist time.
        /*
        */
        ofstream fout1;
        filename = "test_fout1";
        fout1.open(filename); // It will cover last report, if there is any.



        int sec1 = sec_time_upper_bound(num);    //(end_time - start_time) / num
        int ran = rand();
        int sec_gap = ((ran) % sec1 + 1);        //MAY OR MAY NOT BE RIGHT BECAUSE OF THE UNCERTAIN FRONT COEFFICIENT
        Time* time_pointer_gap = pointer_time_upper_bound(sec_gap);
        // fout1<<"previous_last_reg"<<last_reg->year << " "<< last_reg->month << " " << last_reg->day << " " <<last_reg->hour << " " <<last_reg->minute << " " <<last_reg->second<<endl;
        
        last_reg = last_reg->operator+(time_pointer_gap); 

        // fout1<< "ran"<<ran<<endl;
        // fout1<<"sec1" <<sec1<<endl;
        // fout1<<"sec_gap"<< sec_gap << endl;
        // fout1<< "time_pointer_gap"<< time_pointer_gap->year << " "<< time_pointer_gap->month << " " << time_pointer_gap->day << " " <<time_pointer_gap->hour << " " <<time_pointer_gap->minute << " " <<time_pointer_gap->second<<endl;
        // fout1<<"updated_last_reg"<< last_reg->year << " "<< last_reg->month << " " << last_reg->day << " " <<last_reg->hour << " " <<last_reg->minute << " " <<last_reg->second<<endl;
        // fout1.close();
        // do{
		// 	try_time++; // It should not exceed 1000.
		// 	if (try_time > 1000){
		// 		fout.close();
        //         cout << "To much time for setting date" << endl;
        //         exit(EXIT_FAILURE);
		// 	}

		// 	reg_year = 2019 + rand() % 3;
		// 	reg_month = 1 + rand() % 12;
		// 	reg_day = 1 + rand() % 30;
		// 	reg_hour = rand() % 24;
		// 	reg_min = rand() % 60; 
		// 	reg_sec = rand() % 60;
            
		// 	cur_reg->year = reg_year;
		// 	cur_reg->month = reg_month;
		// 	cur_reg->day = reg_day;
		// 	cur_reg->hour = reg_hour;
		// 	cur_reg->minute = reg_min;
		// 	cur_reg->second = reg_sec;
		// } while(last_reg->operator> (cur_reg));
		// last_reg = cur_reg;


        /**********************************/
        
        fout << id << endl;
        fout << name << endl;
        fout << loc1 << " " << loc2 << endl;
        fout << phone_number << endl;
        fout << wechat << endl;
        fout << email << endl;
        fout << pro_cat << endl;
        fout << birth_year << " " << birth_month << " " << birth_day << endl;
        fout << age << endl;
        fout << risk << endl;
        //fout << reg_year << " " << reg_month << " " << reg_day << " " << reg_hour << " " << reg_min << " " << reg_sec << endl << endl;
        fout << last_reg->year << " " << last_reg->month << " " << last_reg->day << " " << last_reg->hour << " " << last_reg->minute << " " << last_reg->second << endl;
        fout << rand() % 3 << endl << endl; // Treatment.
    }

    fout.close();
    return;
}