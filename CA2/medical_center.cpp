#include "CA1.h"
using namespace std;
#include <iostream>

Medical_center::Medical_center(int num_hospital, vector<pair<int, int>> *locations)
{
    num_locations = num_hospital;
    address_location = locations;
    location_lists = new vector<vector<Person*>>(num_locations, vector<Person*>(0, NULL));
    location_ca = new vector<int>(num_locations, 0);
    location_time = new vector<vector<pair<int, int>>>(num_locations, vector<pair<int, int>>(8, make_pair(0, 0)));
    int time_slots = 1;
    for(auto& val: *location_time){
        for(int i = 0; i < val.size(); ++i){
            val[i].second = time_slots++;
        }
    }
}

Medical_center::~Medical_center()
{
    delete[] location_lists;
    delete[] location_ca;
    delete[] location_time;
}

void Medical_center::receive (Person *person, int index, Date *today)
{
    // To set the specific appointment time for "person"

    // "index" refers to the index of the specific hospital with people sent
    // "today" is the appointed day.
    Time *appt = new Time();
    appt->year = today->year;
    appt->month = today->month;
    appt->day = today->day;
    //int person_idx = 0;
    int time_idx = 1, time;

    //push in the person
    (*location_lists)[index].push_back(person);

    // for(; person_idx < location_lists[index].size(); ++person_idx){
    //     if ((*location_lists)[index][person_idx] != NULL){
    //         (*location_lists)[index][person_idx] = person;
    //         break;
    //     }
    // }

    //update the patient numbers in that location
    (*location_ca)[index]++;

    //determine how long does the treatment last for this person
    switch(person->risk)
    {
        case 0:
        case 1:
            time = 1;
            break;
        case 2:
        case 3:
            time = 2;
            break;
    }
    if(time == 1){
        for(; time_idx <= 8; time_idx++){
            if((*location_time)[index][time_idx-1].first <= 9){     //At a specific time, one hospital can cantain at most 10 people. 
                (*location_time)[index][time_idx-1].first++;       //add a person
                break;
            }
        }
    }
    else if(time == 2){
        //for(; i < (((*location_time)[index]).size() - 1); i++){
        for(; time_idx <= (((*location_time)[index]).size() - 1); time_idx++){
            //for people need two hours' treatment, we should guarantee that number of people don't exceed 10 for a period of consecutive two hours.
            if((*location_time)[index][time_idx-1].first <= 9 && (*location_time)[index][time_idx].first <= 9){   
                (*location_time)[index][time_idx-1].first++;   //add a person
                (*location_time)[index][time_idx].first++;
                break;
            }
        }
    }
    
    if((time_idx >= 1) && (time_idx <= 4)){
        appt->hour = time_idx + 7;
    }
    else if((time_idx >= 5) && (time_idx <= 8)){
        appt->hour = time_idx + 8;
    }
    
    Time* one_day = new Time;
    one_day->year = 0;
    one_day->month = 0;
    one_day->day = 1;   // Define one day.
    one_day->hour = 0;
    one_day->minute = 0;
    one_day->second = 0; 

    person->appointment_time = appt;
    person->appointment_time = person->appointment_time->operator+(one_day);    //add another day

}

void Medical_center::delete_person(Person *person)
{
    // Someone withdraw from medical_center.
    //first find the person
    int location_idx = 0;
    int person_idx = 0;
    int found = 0;
    for(; location_idx < location_lists->size(); ++location_idx)
    {
        person_idx = 0;
        for(; person_idx < ((*location_lists)[location_idx]).size(); ++person_idx)
        {
            if((*location_lists)[location_idx][person_idx]->identification_number == person->identification_number)
            {   
                found = 1;
                break;
            }
        }
        if(found == 1) break;
    }
    if(found == 1){
        //delete the person from the vector
        (*location_lists)[location_idx].erase((*location_lists)[location_idx].begin() + person_idx);

        // for(int i = person_idx; i < (*location_lists)[location_idx].size(); ++i)
        // {
        //     (*location_lists)[location_idx][i] = (*location_lists)[location_idx][i+1];
        // }
        person->status = WITHDRAWN;
    }
    else{
        cout<<"Error! This person hasn't received an appointment."<<endl;
    }
}

bool Medical_center::is_free(int index, Person *person)
{
    if(capacity_free(index) && time_free(index, person)) return true;
    else {
        return false;
    }
}

bool Medical_center::capacity_free(int index)
{
    // Check if this location may still have available place for treatment.
    if((*location_ca)[index] <= 80) return true;
    else {
        //cout << "capacity" << endl;
        return false;
    }
    
}

bool Medical_center::time_free(int index, Person *person)
{
    // Check if this location still have available place for treatment.
    // Time slots: 8:00-12:00 1:00-5:00 (1-8) 1 indicates 8:00a.m. ... 8 indicates 4:00p.m.
    int time; // The needed treatment time of "person".
    switch(person->risk)
    {
        case 0:
        case 1:
            time = 1;
            break;
        case 2:
        case 3:
            time = 2;
            break;
    }
    if(time == 1){
        for(int i = 0; i < ((*location_time)[index]).size(); ++i){
            if((*location_time)[index][i].first <= 9) {
                return true;
            }
        }
    }
    else if(time == 2){
        for(int i = 0; i < (((*location_time)[index]).size() - 1); ++i){
            if((*location_time)[index][i].first <= 9 && (*location_time)[index][i+1].first <= 9) return true;
        }
    }
    //cout << "time" << endl;
    return false;
}

void Medical_center::daily_update(void)
{
    for(auto& val: *location_lists){
        val.clear();
    }
    for(auto& val: *location_ca){
        val = 0;
    }
    for(auto& val: *location_time){
        for(auto& people: val){
            people.first = 0;
        }

    }
}

// Medical_center::~Medical_center()
// {
//     delete[] location_ca;
//     delete[] address_location;

//     for (auto x : *location_lists) delete[] &x;
//     delete[] location_lists;
//     for (auto val : *location_time) delete[] &val;
//     delete[] location_time;

// }
