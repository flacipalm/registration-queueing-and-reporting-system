#include "CA2.h"
#include <fstream>
#include <iostream>
#include <string>
#include "fibonacci.cpp"
#include "time.cpp"
#include "local_queue.cpp"
#include "localset.cpp"
#include "hashset.cpp"
#include "hash_chaining.cpp"
#include <algorithm>
#include "Btree_template.cpp"
#include "Bplustree.cpp"

using namespace std;


void Main_data::register_file(const char filename[][51], int file_num){
    static int order = 0;
    for (int i = 0; i < file_num; i++)
    {
        cout << "Loading from registration file..." <<endl << endl;
        Local_Registration_Queue* new_local_queue = new Local_Registration_Queue();
        FILE *file = fopen(filename[i], "r");
        // how many person? 
        fscanf(file, "%d", &new_local_queue->num_of_people_waiting);
        int temp = new_local_queue->num_of_people_waiting;
        total_num += temp;
        // cout << "Now display the person in file " << i + 1 << endl;
        // cout << "There are " << new_local_queue->num_of_people_waiting << " people in this queue" << endl;
        // cout << "=========================================================" << endl;
        // cout << "Now print out the information" << endl;
        while (temp-- > 0) {
            Person *new_person = new Person();
            // read id number
            fscanf(file, "%d", &new_person->identification_number);
            // cout << "The id_number of the person is " << new_person->identification_number << endl;
            // read name
            if (new_person->identification_number == 0){
                cout << "Oh No!" << "In file " << i+1 <<" The" << temp << " Name: " << new_person->name <<endl;
            }
            getc(file);
            fgets(new_person->name, 20, file);
            // cout << "The name of the person is " << new_person->name;
            // read address
            fscanf(file, "%d %d", &new_person->address.first, &new_person->address.second);
            // cout << "The address of the person is " << new_person->address.first << " " << new_person->address.second << endl;
            // read phone number
            getc(file);
            fgets(new_person->phone, 20, file);
            // cout << "The phone number of the person is " << new_person->phone;
            // read wechat id
            fgets(new_person->wechat, 40, file);
            // cout << "The wechat id of the person is " << new_person->wechat;
            // read email
            fgets(new_person->email, 40, file);
            // cout << "The email of the person is " << new_person->email;
            // profession category
            fscanf(file, "%d", &new_person->professional_category);
            // cout << "The profession category of the person is " << new_person->professional_category << endl;
            // birth date
            new_person->birth_date = new Date;
            fscanf(file, "%d %d %d", &new_person->birth_date->year, &new_person->birth_date->month, &new_person->birth_date->day);
            // cout << "The birth date of the person is " << new_person->birth_date->year << " " << new_person->birth_date->month << " " << new_person->birth_date->day << endl;
            // age
            fscanf(file, "%d", &new_person->age);
            // cout << "The age of the person is " << new_person->age << endl;
            // Risk level
            fscanf(file, "%d", &new_person->risk);
            // cout << "The risk level of the person is " << new_person->risk << endl;
            // time and date of registration
            new_person->date = new Time;
            fscanf(file, "%d %d %d %d %d %d", &new_person->date->year, &new_person->date->month, &new_person->date->day, &new_person->date->hour, &new_person->date->minute, &new_person->date->second);
            // cout << "The time and date of registration is " << new_person->date->year << " " << new_person->date->month << " " << new_person->date->day << " " << new_person->date->hour  << " " << new_person->date->minute << " " << new_person->date->second << endl;
            // convert age
            new_person->convert_age_to_cat();
            // =========================== New here, add treatment type ================================
            int treatment_type;
            getc(file);
            fscanf(file, "%d", &treatment_type);
            if (treatment_type == 0) new_person->treatment = VACCINATION;
            else if (treatment_type == 1) new_person->treatment = CHEMOTHERAPY;
            else new_person->treatment = SURGERY;
            // Create deadline (empty at the beginning)
            new_person->deadline = new Time();
            // Put them in the database.
            Tuple *relation = new Tuple(new_person);
            insert_tuple(*relation, 1);
            // cout << endl;
        }
        // Insert in B-tree, B+-tree or hashtable
    }

    display();

    //B+tree insertion.
    BPTree bptree = BPTree();
    for (auto cur_block : all_data) {
        for (int i = 0; i < cur_block.size; i++)
        {
            pair<int, int> insert_pair = make_pair(cur_block.block_data[i].id, cur_block.block_data[i].block_idx);
            bptree.insert(insert_pair);
        }

        for (int j = 0; j < cur_block.overflow.size; j++) 
        {
            pair<int, int> insert_pair2 = make_pair(cur_block.overflow.block_data[j].id, cur_block.overflow.block_data[j].block_idx);
            bptree.insert(insert_pair2);
        }
    }
    bptree.display_only_leaf(bptree.getRoot());
    

    // Hashtable insertion.
    hash_chaining hash = hash_chaining();
    for(auto cur_block: all_data){
        for(int i = 0; i < cur_block.size; i++){
            hash.insert_tuple(cur_block.block_data[i]);
        }

        for (int j = 0; j < cur_block.overflow.size; j++) 
        {
            hash.insert_tuple(cur_block.overflow.block_data[j]);
        }
    }
    hash.display();


    // B-tree Insertion.
    Btree tree(5);
    for (auto cur_block : all_data) {
        for (int i = 0; i < cur_block.size; i++)
        {
            pair<pair<int, int>, int> insert_pair = make_pair(make_pair(cur_block.block_data[i].id, hash.hash_func(cur_block.block_data[i])),cur_block.block_data[i].block_idx);
            tree.insert(insert_pair);
        }

        for (int j = 0; j < cur_block.overflow.size; j++) 
        {
            pair<pair<int, int>, int> insert_pair2 = make_pair(make_pair(cur_block.overflow.block_data[j].id, hash.hash_func(cur_block.overflow.block_data[j])), cur_block.overflow.block_data[j].block_idx);
            tree.insert(insert_pair2);
        }
    }
    tree.traverse();
}

void Main_data::register_withdraw(int num_id, Time *now, Time *rereg, Local_set *all_queue)
{
    Person *item_found;
    if ((item_found = database->member(num_id)) == NULL) {
        cout << "Error! This person was not in the database!" << endl;
        exit(0);
    }
    if (now->operator==(rereg))
    {
        item_found->status = REGISTERED;
        Time *add = new Time();
        add->day = 14;
        Time *new_time = rereg->operator+(add);
        item_found->date = new_time;
        // Add this person into the first local queue
        (*all_queue->local_queues)[0]->add_person(item_found);
    }
    return;
}

void Main_data::adjust_risk(int id_num, Time *now, Time *set, int new_risk, Central_Queue *all_queues){
    if (now->operator==(set)){
        Person *item_found;
        if ((item_found = database->member(id_num)) == NULL) return;
        if (new_risk > item_found->risk) return;
        item_found->risk = new_risk;
        if (item_found->status == WAITING) all_queues->adjust_attr(item_found);    
    }
    return;
}

void Main_data::adjust_pro(int id_num, Time *now, Time *set, int new_pro, Central_Queue *all_queues){
    if (now->operator==(set)){
        Person *item_found;
        if ((item_found = database->member(id_num)) == NULL) return;
        if (item_found->professional_category > new_pro){
            // Priority changes.
            item_found->professional_category = new_pro;
            if (item_found->status == WAITING){
                all_queues->adjust_attr (item_found); // Adjust the Fib when someone's risk status/ professional category has changed.
            }
        }
    }
    return;
}

void Main_data::set_ddl(int id_num, Time *now, Time *set, Time* ddl, Central_Queue *all_queues){
    if (now->operator==(set)){
        // cout << now->year << " " << now->month << " " << now->day << " " << endl;
        int diff = convert_difference_to_days(ddl, set);
        if (diff <= 1){
            cout << "Fail to set ddl!!!";
            return;
        }
        Person *item_found;
        if ((item_found = database->member(id_num)) == NULL) return;
        item_found->deadline = ddl;
        if(item_found->status == WAITING){
            all_queues->delete_person(item_found);
            all_queues->add_severe(item_found);
        }
    }
}

void Main_data::delete_withdrawn(int id_num, Time *now, Time *set, Central_Queue *center_queue, Local_set *all_queue, Medical_center *md_ctr)
{
    if (now->operator==(set))
    {
        // cout << "executing delete_withdraw!" << endl;
        Person *item_found;
        if ((item_found = database->member(id_num)) == NULL) return;
        if (item_found->status == COMPLETED) 
        {
            cout << "Cannot withdraw this person since he has already been treated!" << endl;
            exit(0);
        }
        if (item_found->status == REGISTERED) 
        {
            item_found->status = WITHDRAWN;
            // iterate through the local set
            for (auto local_queue : *all_queue->local_queues)
            {
                for (int y = 1; y <= local_queue->local_queue->getlength(); y++) 
                {
                    if ((*local_queue->local_queue)[y] == item_found) local_queue->local_queue->deletion(item_found);
                }
            }
            return;
        }
        if (item_found->status == WAITING)
        {
            // Withdraw from central queue.
            item_found->status = WITHDRAWN;
            // cout << item_found->identification_number << endl;
            center_queue->delete_person(item_found);
            return;
        }
        if (item_found->status == APPOINTED) 
        {
            // Withdraw from medical center.
            item_found->status = WITHDRAWN;
            md_ctr->delete_person(item_found);
            return;
        }
    }
    return;
}

void Main_data::treatment_complete(Time* today)
{
    // Daily check the people who have finished treatment.
    for (auto x : *collection){
        if (x->status == APPOINTED && (x->appointment_time->operator<=(today))){
            x->status = COMPLETED;    
        }
    }
}

void Main_data::report_treated(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
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

void Main_data::report_with_appointment(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
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

void Main_data::report_without_appointment(Time *now, int past_week, bool by_name, bool by_profession, bool by_age)
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

void Main_data::monthly_report(Time *now, int past_month, Local_set *localset){
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

void Main_data::insert_block(Block block){
    all_data.push_back(block);
    block_num++;
}

void Main_data::delete_block(int i){
    all_data.erase(all_data.begin() + i);
    block_num--;
}

void Main_data::insert_tuple(Tuple relation, int mode){
    int i;  // "i" is the index of the block to be inserted.
    if (mode == 0){
        // Insert at the end.
        i = block_num - 1;
    }
    else{
        // Insert uniformly.
        i = total_num % block_num;
    }
    
    relation.block_idx = i;
    all_data[i].overflow.insert(relation);
    // cout << all_data[i].overflow.size << endl;
    if (all_data[i].overflow.size == all_data[i].overflow.max_size){
        all_data[i].overflow.sort_block();
        // The overflow block is full.
        if (all_data[i].size + all_data[i].overflow.size >= all_data[i].fix_size){
            // The block is full.
            //"split" create a new block at the end of the main_data and store the second half f the tuple st(the number of tuple is == or +1 compared to the first half) o the new block  
            split(i);
        }
        all_data[i].merge(); // Merge the block with its overflow block.(now the block stores the first half of the tuple as well as tuples over flow blocks)
        // cout << all_data[i].size << endl;
    }
    total_num++;
}

void Main_data::delete_tuple(int i, int id){
    if (all_data[i].delete_tuple(id) != 0){
        if (all_data[i].size + all_data[i].overflow.size <= all_data[i].threshold){
            all_data[i].merge(); // Put all the data in overflow block to the block;
            merge(i); // Merge the block with the last block in the main_data.
        }
    }
    total_num--;
}

void Main_data::merge(int i)
{
    // Merge i with the last block.
    if (i == block_num - 1){
        // This is the last block, we neglect this procedure.
        return;
    }
    for (int j = 0; j < all_data[i].size; j++){
        insert_tuple(all_data[i].block_data[j], 0);
        total_num--; // Total_num++ in the procession of "insert_tuple".
    }
    // The block is empty now.
    delete_block(i);
    return;
}

void Main_data::split(int i){
    // Create a new block at the end of the main_data.
    if (i >= block_num ){
        printf("The value of the index of the block to be split is out of range!!!");           //*********when will this condition happens?********
        exit(0);
    }
    else{
        Block* new_block = new Block();
        insert_block(*new_block); // 
        for (int j = all_data[i].size/2; j < all_data[i].size; j++){
            // Cut the second half of the block to the new block.
            insert_tuple(all_data[i].block_data[j], 0);
            total_num--; // Total_num++ in the procession of "insert_tuple".
        }
        all_data[i].size /= 2; // The size of the original block is half.
    }
    return;
}   

void Main_data::display(){
    // Display ids in main_data.
    cout << "There are totally " << block_num << " blocks." << endl;
    for (int i = 0; i < block_num; i++){
        cout << "In block " << i << ", there are " << (all_data[i].size + all_data[i].overflow.size) << " people" << endl;
        // for (int j = 0; j < all_data[i].size; j++){
        //     cout << all_data[i].block_data[j].id << endl;
        // }
    }
    cout << endl;
    return;
}