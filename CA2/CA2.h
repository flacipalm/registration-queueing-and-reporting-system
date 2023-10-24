#ifndef CA2_h
#define CA2_h

#include <iostream>
#include <algorithm>
#include <fstream>
#include "CA1.h"



// =============================================================
/********************Specify for CA 2********************/
// ================================================================

// A "tuple" that stores four relations of a person
class Tuple{
public:
    Tuple(Person *p) {
        // Constructor
        id = p->identification_number;
        status = p->status;
        date = p->date;
        pro = p->professional_category;
        risk = p->risk;
        age = p->age_cat;
    } 
    //
    Tuple(){} // Default constructor
    int id; // PERSON 
    Status status; // MEDICAL_STATUS
    Time *date; // REGISTRATION TIME
    Treatment treatment; // TREATMENT TYPE
    int pro;
    int risk;
    int age;
    int block_idx; // WHICH BLOCK STORES THE TUPLE
};

class Overflow_block
{
public:
    Overflow_block()
    {
        block_data = vector<Tuple>(0); // how to call default constructor???
        
        for (int i = 0; i < max_size; i++) {
            Tuple *new_tuple = new Tuple();
            block_data.push_back(*new_tuple);
        }
        size = 0;
    }

    vector<Tuple> block_data;
    int max_size = 20;
    int size;

    void insert(Tuple relation);

    int del(int id);

    void sort_block(void);
    
    static bool comparison_functor(Tuple a, Tuple b)
    {
        if (a.id > b.id) return 0;
        return 1;
    }
};

class Block{
public:
    // Constructor
    Block()
    {
        block_data = vector<Tuple>(0);
        fix_size = 120;
        for (int i = 0; i < fix_size; i++) {
            Tuple *new_tuple = new Tuple();
            block_data.push_back(*new_tuple);
        }
        size = 0;
        threshold = 10;
    }

    vector<Tuple> block_data; // A vector to store the tuples of people, its maxsize is determined by "fix_size".
    int fix_size;       
    int threshold; // Merge operation will be invoked when both blocks have size below the threshold value
    int size;                 // Current size of "block_data".
    Overflow_block overflow;

    // Delete a tuple with a certain "id" from this block.
    // 0 means no tuple, 2 means overblocks, 1 means main data
    int delete_tuple(int id);
    
    // merge the main block with the overflow block
    // notice that the data in the blocks are all continous
    void merge(void);
    
    void insert(Person *person);
};

class Main_data
{
public:
    Main_data(){
        block_num = 1;
        total_num = 0;
        all_data = vector<Block>(0);
        Block first_bk = Block();
        all_data.push_back(first_bk);
    }
    // store all the blocks 
    vector<Block> all_data;
    int block_num;
    int total_num;

    // functions migrated from CA1
    void register_file(const char filename[][51], int file_num);
    void report_treated(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);
    void report_with_appointment(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);
    void report_without_appointment(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);

    void monthly_report(Time *now, int past_month, Local_set *localset);
    // Add people to the database from file
    // And assign them into the local registry queue
    void register_withdraw(int num_id, Time *now, Time *rereg, Local_set *all_queue);

    // compact information used for sorting
    vector<Person*> *collection;

    // Use large enough array to store ALL people's data pointer
    // Large enough to avoid rehashing as possible
    Hashset *database;

    
    void treatment_complete(Time* today);
    // functions for calculation
    // Print time for a single person (boolean variable specifies which time, ONLY ONE IS MARKED "true")
    void print_time(int person_id, bool treatment, bool waiting_till_now);
    void print_average_waiting_time(void);

    void adjust_risk(int id_num, Time *now, Time *set, int new_pro, Central_Queue *all_queues);
    void adjust_pro(int id_num, Time *now, Time *set, int new_pro, Central_Queue *all_queues);
    void set_ddl(int id_num, Time *now, Time *set, Time* ddl, Central_Queue *center_queue);
    void delete_withdrawn(int id_num, Time *now, Time *set, Central_Queue *center_queue, Local_set *all_queue, Medical_center *md_ctr);

    // Information needed in reporting
    int num_people_all;
    int num_people_inline;
    int num_people_treated; // in total
    int num_people_withdrawed; // in total
    int num_of_person_waiting; // all queues

    // helper functions in comparison
    static bool sort_name(Person *first, Person *second) 
    {
        const char *first_name = first->name;
        const char *second_name = second->name;
        if (strcmp(first_name, second_name) < 0) return 1;
        return 0;
    }
    static bool sort_prof(Person *first, Person *second) {return first->professional_category < second->professional_category;}
    static bool sort_age(Person *first, Person *second) {return first->age < second->age;}

    
    // Add a block at the end of the main_data.
    void insert_block(Block block);

    // Delete the block with index "i" from the main_data.
    void delete_block(int i);
    
    
    void insert_tuple(Tuple relation, int mode);


    // Delete a tuple with "id" from the block with index "i".
    void delete_tuple(int i, int id);


    // Merge the block with index "i" with the last block.
    void merge(int i);

    // Split a block with index "i" into two.              
    // primary key copied
    void split(int i);

    void display();
}; 

#endif