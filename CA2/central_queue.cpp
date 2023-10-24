#include "CA1.h"
#include "medical_center.cpp"
#define INFINITY 1000000000
using namespace std;
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;
Central_Queue::Central_Queue()
{
    Center_fib_heap = new Fib_heap();
    severe_per = new vector<Person*>(0);
}

void Central_Queue::print_rootlist(void)
{
    ofstream rootlist;
    rootlist.open("rootlist.txt"); 
    Fib_node *cur_min = Center_fib_heap->min;
    do {
        rootlist << "   ID: " << cur_min->value->identification_number;
        rootlist << "   Profession Category: " << cur_min->value->professional_category;
        rootlist << "   Age Category: " << cur_min->value->age_cat;
        rootlist << "   Risk Status: " << cur_min->value->risk;
        rootlist << "   Status: " << cur_min->value->status;
        rootlist << endl << endl;
        cur_min = cur_min->right;
    } while (cur_min != Center_fib_heap->min);
    rootlist.close();
}

void Central_Queue::add_person(Local_set *local_set)
{   
    // ofstream localfile;
    // localfile.open("localfile.txt");
    for(auto val : *(local_set->people_selected))
    {

        val->status = WAITING;
        if((val->deadline->year == 0) && (val->deadline->month == 0) && (val->deadline->day == 0)
            && (val->deadline->hour == 0) && (val->deadline->minute == 0) && (val->deadline->second == 0))
            {   
                // localfile << "   ID: " << val->identification_number;
                // localfile << "   Profession Category: " << val->professional_category;
                // localfile << "   Age Category: " << val->age_cat;
                // localfile << "   Risk Status: " << val->risk;
                // localfile << "   Status: " << val->status;
                // localfile << endl << endl;
                Center_fib_heap->insert(val);
            }
        else{   add_severe(val); }
    }
}

void Central_Queue::add_severe(Person* person)
{
    // Place people with ddl into the sorted vector.
    severe_per->push_back(person);
    sort(severe_per->begin(), severe_per->end(), sort_ddl);
}

int Central_Queue::send (Person *person, Medical_center *medctr, Time *today)
{
    // The person to be sent should be determined in advance, use helper functions below
    // (first for those with ddl at that day and then those in fibonacci heap)
    int per_a = person->address.first;
    int per_b = person->address.second;
    // location of appointment
    int dest_a, dest_b;
    // Minimal distance found now
    int min_dist_sq = INFINITY;
    // current hospital and current distance from addr of the person to the hospital
    int temp_idx = -1;
    // current SQUAREED distance from the addr of that person to the current hospital
    int temp_dist_sq;

    for(int idx = 0; idx < medctr->num_locations; ++idx){
        pair<int, int> val = (*(medctr->address_location))[idx];
        dest_a = val.first;
        dest_b = val.second; // Coordinates of the treatment location
        temp_dist_sq = (dest_a - per_a)*(dest_a - per_a) + (dest_b - per_b)*(dest_b - per_b);
        if(temp_dist_sq < min_dist_sq){
            // cout << "distance: " << temp_dist_sq << endl;
            //to be supplemented: only when the location is free, send the person
            if(medctr->is_free(idx, person)){
                min_dist_sq = temp_dist_sq;
                temp_idx = idx;
            }
        }
    }

    // Nearest hospital found
    if(temp_idx != -1){
        Time *appt = new Time();
        medctr->receive(person, temp_idx, today);
        
        delete_person(person); // Only delete when succeed in pushing
        // Delete here or somewhere else?!
        person->status = APPOINTED;
        return 1; // Indicate successful sending;
    }
    else 
    {
        // cout << "unsuccessful" << endl;
        return 0; //unsuccessful
    }
        
}

void Central_Queue::send_severe(Medical_center *medctr, Time *today)
{
    /*we make the assumption that those whose ddl is exactly today are able to 
        make an appointment successfully. Since everyday, up to 80 patients can be 
        sent in total.
    */
    Time *send_ddl = new Time();
    Time *period = new Time();
    period->day = 1;
    
    for(auto val : *(this->severe_per)){
        send_ddl = val->deadline->operator-(period);

        if((send_ddl->year == today->year) && (send_ddl->month == today->month) && (send_ddl->day == today->day ))
        {
            send(val, medctr, today);
            //only send those whose ddl is exactly today
        }
        else{ break; }
    }
}

int Central_Queue::send_norm_from_heap (Medical_center *medctr, Time *today)
{
    Person *pts = Center_fib_heap->min->value;
    int suc = send(pts, medctr, today);
    return suc;
}

Fib_node *Central_Queue::search_fibheap(Fib_node *tempmin, Person *k)
{
    Fib_node *temp = tempmin;
    // cout << temp->value->identification_number << endl;
    Fib_node *ptk = NULL;
    if( (temp != NULL) ) /*&& (k->operator>(temp->value) || k->operator==(temp->value) || k->isequal(temp->value)) )*/
    {
        // cout << "enter if" << endl;
        
        do{ 
            if (k->operator==(temp->value)) {
                ptk = temp; 
                return ptk;
            }
            
            ptk = search_fibheap(temp->left_most_child, k);  
            
            if(ptk != NULL) return ptk;
            temp = temp->right;
        } while(temp != tempmin);
    }
    // cout << "Fobidden here" << endl;
    return NULL;
}

// int Central_Queue::confirm(Person *node_to_check, int root_num, int depth, int column)
// {
//     int copy_rootnum = root_num;
//     int copy_depth = depth;
//     int copy_column = column;
//     Fib_node *start_root = Center_fib_heap->min;
//     while (--copy_rootnum) {
//         start_root = start_root->right;   
//     } 
//     Fib_node *start_left_child = start_root->left_most_child;
//     if (copy_depth == 1) start_left_child = start_root;
//     else {
//         copy_depth--;
//         int count = 2;
//         while (--copy_depth) {
//             cout << ++count << endl;
//             start_left_child = start_left_child->left_most_child;
//         }
//     }
//     while (copy_column--) {
//         start_left_child = start_left_child->right;
//     }
//     if (node_to_check->operator==(start_left_child->value)) {
//         return 1;
//     }
//     cout << "node_to_check's ID: " << node_to_check->identification_number << endl;
//     cout << "start_left_child->value's ID: " << start_left_child->value->identification_number << endl; 
//     return 0;
    
// }

void Central_Queue::delete_person(Person *person)
{
    //first find whether the person is in the heap
    // If not, find and delete the person in severe (vector)
    // cout << person->identification_number << endl;
    Fib_node *ptp = search_fibheap(Center_fib_heap->min, person);
    if (ptp != NULL) {
        // cout << ptp->value->identification_number << endl;
        // cout << Center_fib_heap->min->value->identification_number << endl;
        // cout << (ptp == Center_fib_heap->min) << endl;
        Center_fib_heap->deletenode(ptp);
        // if (Center_fib_heap->min != NULL) cout << "HAHAHAHA" << endl;
    } 
    else {
        // cout << "Searching for people with ddl..." << endl;
        delete_severe(person);
    }
}

void Central_Queue::delete_severe(Person *person)
{
    int i = 0;
    for(; i < severe_per->size(); ++i)
    {
        //first find the index where stores the person
        if((*severe_per)[i]->identification_number == person->identification_number) break;
    }
    severe_per->erase(severe_per->begin() + i);
}

// void Central_Queue::adjust_risk (Person *person, Time *now, Time *set, int new_risk)
void Central_Queue::adjust_attr (Person *person)
{
    //if(new_risk >= person->risk) return;
    Fib_node *ptp = search_fibheap(Center_fib_heap->min, person);
    //item_found->risk = new_risk;
    Center_fib_heap->adjust_priority(ptp);
}

// // void Central_Queue::adjust_risk (Person *person, Time *now, Time *set, int new_pro)  
// void Central_Queue::adjust_pro(Person *person)
// {
//     //if(new_pro >= person->professional_category) return;
//     Fib_node *ptp = search_fibheap(Center_fib_heap->min, person);
//     //person->professional_category = new_pro;
//     Center_fib_heap->adjust_priority(ptp);
// }
    
void Central_Queue::find_severe (Person *person)
{
    Fib_node *ptp = search_fibheap(Center_fib_heap->min, person);
    if(ptp == NULL){
        cout << "This person isn't waiting in the central queue." <<endl;
        return;
    } 
    else
    {
        Center_fib_heap->deletenode(ptp);
        add_severe(person);
    }

}