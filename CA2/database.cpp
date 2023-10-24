#include <fstream>
#include <iostream>
#include <string>
#include "fibonacci.cpp"
#include "time.cpp"
#include "local_queue.cpp"
#include "localset.cpp"
#include "hashset.cpp"
#include <algorithm>   


void Person_Database::report_treated(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
{
    /* /
        Report the people who have been treated including their profession category, age category,
        risk status and the waiting time from registration to treatment.
    */

    if (by_name + by_profession + by_age != 1) {
        cout << "Only one sorting criteria should be chosen!";
        exit(1);
    }
    vector<Person*> *treated = new vector<Person*>(0);
    for (auto x : *collection) if (x->status == COMPLETED) treated->push_back(x);
    // cout << treated->size() << endl;
    // Sort by specified
    // Person *test_prev = (*treated)[5];
    if (by_name) {
        sort(treated->begin(), treated->end(), sort_name);
        // cout << "name" << endl;
    }
    else if (by_profession) {
        sort(treated->begin(), treated->end(), sort_prof);
        // cout << "profession" << endl;
    }
    else if (by_age) {
        sort(treated->begin(), treated->end(), sort_age);
        // cout << "age" << endl;
    }
    // Person *test_after = (*treated)[5];
    // cout << test_prev->operator==(test_after) << endl;
    // Print out the result in a file
    Time* diff = new Time; // Time difference.
    ofstream fout;
    int order = 0; // The order of people in report.
    string filename = "Report/Weekly/Treated/Week" + to_string(past_week) + ".txt";
    fout.open(filename); // It will cover last report, if there is any.
    fout << "Reporting of the People who have been treated" << endl << endl; 
    for (auto one : *treated){
        // One person per line.
        order++;
        diff = subtract_time(one->appointment_time, one->date);

        fout << order << ". " << one->name;
        fout << "   Appointment_time: " << one->appointment_time->year <<" "<< one->appointment_time->month<<" "<<one->appointment_time->day<<" "<<one->appointment_time->hour<<" "<<one->appointment_time->minute <<" "<< one->appointment_time->second << endl;
        fout << "   Date: " << one->date->year<<" " << one->date->month<<" "<<one->date->day<<" "<<one->date->hour<<" "<<one->date->minute<<" "<<one->date->second<<endl;
        fout << "   ID: " << one->identification_number;
        fout << "   Profession Category: " << one->professional_category;
        fout << "   Age Category: " << one->age_cat;
        fout << "   Risk Status: " << one->risk;
        fout << "   Waiting Time from Registration to Treatment: " << (diff->year > 0 ? to_string(diff->year) + (diff->year > 1 ? " years " : " year ") : ""); // Nested ternary operators.
        fout << (diff->month > 0 ? to_string(diff->month) + (diff->month > 1 ? " months " : " month ") : "");
        fout << to_string(diff->day) + (diff->day > 1 ? " days" : " day");
        fout << endl << endl;
    }
    fout.close();

    return;

}

void Person_Database::report_with_appointment(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
{
    /* 
        Report the registered people with a set appointment including their profession category, age
        category, risk status and their waiting time until now.
    */
    if (by_name + by_profession + by_age != 1) {
        cout << "Only one sorting criteria should be chosen!";
        exit(1);
    }
    vector<Person*> *appointed = new vector<Person*>(0);
    for (auto x : *collection) if (x->status == APPOINTED) appointed->push_back(x);

    // Sort by specified
    if (by_name) sort(appointed->begin(), appointed->end(), sort_name);
    else if (by_profession) sort(appointed->begin(), appointed->end(), sort_prof);
    else if (by_age) sort(appointed->begin(), appointed->end(), sort_age);

    // Print out the result in a file
    Time* diff = new Time; // Time difference.
    ofstream fout;
    int order = 0; // The order of people in report.
    string filename = "Report/Weekly/With_Appointment/Week" + to_string(past_week) + ".txt";
    fout.open(filename); // It will cover last report, if there is any.
    fout << "Reporting of the Queueing People With a Set Appointment" << endl; 
    for (auto one : *appointed){
        order++;
        //diff = now->operator-(one->date);
        diff = subtract_time(now,one->date);
        fout << order << ". " << one->name; 
        fout << "   ID: " << one->identification_number;
        fout << "   Profession Category: " << one->professional_category;
        fout << "   Age Category: " << one->age_cat;
        fout << "   Risk Status: " << one->risk;
        fout << "   Waiting Time: " << (diff->year > 0 ? to_string(diff->year) + (diff->year > 1 ? " years " : " year ") : ""); // Nested ternary operators.
        fout << (diff->month > 0 ? to_string(diff->month) + (diff->month > 1 ? " months " : " month ") : "");
        fout << to_string(diff->day) + (diff->day > 1 ? " days" : " day") << endl;
        fout << endl << endl;
    }
    fout.close();

    return;    
}

void Person_Database::report_without_appointment(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
{
    /* 
        Report the queueing people without a set appointment including their profession category,
        age category, risk status and their waiting time until now.
    */


    if (by_name + by_profession + by_age != 1) {
        cout << "Only one sorting criteria should be chosen!";
        exit(1);
    }
    vector<Person*> *no_appointed = new vector<Person*>(0);
    for (auto x : *collection){
        if ((x->status == REGISTERED && x->date->operator<=(now))|| x->status == WAITING) no_appointed->push_back(x);
        if (x->identification_number == 0){
            cout << "In database" << endl;
        }
    } 
    // Sort by specified
    if (by_name) sort(no_appointed->begin(), no_appointed->end(), sort_name);
    else if (by_profession) sort(no_appointed->begin(), no_appointed->end(), sort_prof);
    else if (by_age) sort(no_appointed->begin(), no_appointed->end(), sort_age);

    // Print out the result in a file

    Time* diff = new Time; // Time difference.
    int diffi;
    ofstream fout;
    int order = 0; // The order of people in report.
    string filename = "Report/Weekly/Without_Appointment/Week" + to_string(past_week) + ".txt";
    fout.open(filename); // It will cover last report, if there is any.
    fout << "Reporting of the Queueing People Without a Set Appointment" << endl; 
    for (auto one : *no_appointed){
        order++;
        diff = subtract_time(now, one->date);
        diffi = convert_difference_to_days(now,one->date);
        fout << order << ". " << one->name;
        fout << "   ID: " << one->identification_number;
        fout << "   Profession Category: " << one->professional_category;
        fout << "   Age Category: " << one->age_cat;
        fout << "   Risk Status: " << one->risk;
        fout << "   Status: " << one->status;
        fout << "   Waiting Time: " << (diff->year > 0 ? to_string(diff->year) + (diff->year > 1 ? "years " : "year ") : ""); // Nested ternary operators.
        fout << (diff->month > 0 ? to_string(diff->month) + (diff->month > 1 ? "months " : "month ") : "");
        fout << to_string(diff->day) + (diff->day > 1 ? "days" : "day");
        fout << endl << endl;
    }
    fout.close();
    return;
}

void Person_Database::monthly_report(Time *now, int past_month, Local_set *localset){
    /*  
        In addition the system shall produce a monthly statistics report showing how many people
        have registered, how many of them are waiting, how many are waiting in total, how many
        treatment appointments have been made, the average waiting time, and the number of
        people who withdrew their registration.
    */
    
    int waiting_tot = 0; // Total number of people waiting.
    int finished_tot = 0; // Total number of people treated.
    int withdrawn_tot = 0; // Total number of people withdrawn.
    int waiting_central_tot = 0; // Number of people waiting in central queue.
    int waiting_locals_tot = 0; // Number of people waiting in all local queues.
    int waiting_time_tot = 0; // Total waiting time considering all status except "COMPLETED". Day as unit.
    int waiting_time_avg; // Average waiting time. Day as unit.
    int magic = 0;
    
    vector<int> *waiting_locals = new vector<int>((localset->local_queues)->size(),0);       // The vector stores each local registries's condition
    for (auto x : *collection){
        if (((x->status == REGISTERED) && (x->date->operator<=(now) )) || x->status == WAITING || x->status == APPOINTED){
            waiting_tot++;
            if (x->status == WAITING){
                waiting_central_tot++;
            }
            if (x->status == REGISTERED && (x->date->operator<=(now))){
                waiting_locals_tot++;
            }
            if (x->status == APPOINTED){
                magic++;
            }
            waiting_time_tot += convert_difference_to_days(now, x->date);       
        }
        else if (x->status == COMPLETED){
            finished_tot++;
            waiting_time_tot += convert_difference_to_days(x->appointment_time, x->date);
        }
        else if (x->status == WITHDRAWN){
            withdrawn_tot++;
        }
    }
    if (waiting_time_tot == 0){
        waiting_time_avg = 0;
    }
    else{
        // cout << "waiting_tot: " << waiting_tot << endl;
        // cout << "finished_tot: " << finished_tot << endl;
        waiting_time_avg = waiting_time_tot / (waiting_tot + finished_tot); 
    }
    
    // int j;
    // for (int i = 0; i < localset->local_queues->size(); i++){
    //     for (j = 1; j <= ((*localset->local_queues)[i])->local_queue->getlength(); j++){
    //         if ((*(((*(localset->local_queues))[i])->local_queue))[j]->date->operator<=(now)){
    //             (*waiting_locals)[i] += 1;
    //         }
    //     }
    // }
    
    //Print out the result in a file

    ofstream fout;

    string filename = "Report/Monthly/Month" + to_string(past_month) + ".txt";
    fout.open(filename); // It will cover last report, if there is any.
    fout << "Monthly Statistics Report" << endl; 
    fout << "Total number of people that have registered: " << (waiting_tot+finished_tot) << endl;

    // for (int i = 0; i < localset->local_queues->size(); i++){
    //     fout << "The number of people who are waiting in " << i << " local queue: " << (*waiting_locals)[i] << endl;
    // }

    fout << "The number of people who are waiting in central queue: " << waiting_central_tot << endl;
    fout << "The number of people waiting in total: " << waiting_tot <<endl;
    fout << "The number of people withdrawn: " << withdrawn_tot << endl;
    fout << "Total number of treatment: " << finished_tot<<endl;
    fout << "Average waiting Time: " << waiting_time_avg << (waiting_time_avg > 1 ? " days" : " day") << endl;
    fout << "The number of people who has magic: " << magic << endl;
    fout.close();
    return;
}