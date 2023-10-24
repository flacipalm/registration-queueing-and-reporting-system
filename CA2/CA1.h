#ifndef CA1_h
#define CA1_h

#include <stddef.h>
#include <string.h>
#include <vector>

using namespace std;


// Age category enumeration 0-6
enum Age_Category {
    CHILDREN, ADOLESCENT, YOUNG_ADULT, ADULT, SENIOR, ELDERLY_PEOPLE, OLD_PEOPLE, NUM_OF_AGE_CATEGORY
};

// Status of a person
enum Status {
    REGISTERED, WAITING, APPOINTED, COMPLETED, WITHDRAWN, NUM_OF_STATUS
};
/*  
    RGISTERED = In local queue.
    WAITING = In central queue.
    APPOINTED = Allocated an appointment time.
    COMPLETED = Treatment finished.
    WITHDRAWN = Withdrawn from processes.
*/
// Three types of Treatment (Vaccination, with priority rules defined in CA1; Chemotherapy, first come first served; surgery, person with higher risk level has higher priority)
enum Treatment{
    VACCINATION, CHEMOTHERAPY, SURGERY 
};

// class for time and date storage
class Date
{
public:
    int year;
    int month;
    int day;
};

class Time : public Date
{
public:
    Time();
    // Define operators to allow date arithmetics
    Time* operator+(Time *sec);
    Time* operator-(Time *sec);
    bool operator>(Time *sec);
    bool operator<=(Time *sec);
    bool operator==(Time *sec);
    int hour;
    int minute;
    int second;
    
};

// used in monthly / weekly report
int convert_difference_to_days(Time* a, Time* b);

// time point - time point = time interval
Time* subtract_time(Time *sec1, Time * sec2);

// used in input_genetator
int sec_time_upper_bound(int num);
Time* pointer_time_upper_bound(int sec_upper_bound);


class Person
{
public:
    Person() {};
    // Should be unique (used for hashing)
    int identification_number; 
    // Name of a person (whitespace should be allowed)
    char name[21]; 
    // x and y axis of the address, use it to find the nearest location to take vaccine
    pair<int, int> address; 
    // phone, wechat and email of a person (DO NOT EXCEED THE LIMITS)
    char phone[21];
    char wechat[41];
    char email[41];
    // professional category goes from 1 to 8 (may change)
    int professional_category;
    // The age of a person 
    int age; 
    // Seven different age categories (use "age" to determine the age category)
    Age_Category age_cat; 
    // The birth date of a person
    Date *birth_date;
    // Four different risk levels (from 0-3)
    int risk; 

    // time and date of registration 
    Time *date;
    // Status of a person: applie , waiting, allocated(appointed), complete and withdrawn.
    Status status;
    // appointment time and location (number of hospital)
    Time *appointment_time;
    int location;
    Time *deadline;
    Treatment treatment; // TREATMENT TYPE  (0-2)
    /*
    If the person is in no or low risk:
        First by profession category
        Then by age groups
        Last by date and time
    For median risk, 30 days are added when comparison
    For high risk, only consider them when there is no person with median, low or no risk
    */
    // Define priority operator
    bool operator>(Person *second);
    // Two persons are equal iff they have the same id
    bool operator==(Person *second);

    // bool isequal(Person *second);
    // Helper function
    void convert_age_to_cat(void);
};

class fifo
{
public:
    fifo(int size = 10);

    Person* operator[](int index);
    int getlength(void);
    bool isempty(void);
    Person* back(void);
    Person* front(void);
    void pushback(Person* value);
    Person* popfront(void);
    int numitems;
    void deletion(Person* value);
    
private:
    int maxsize, minsize;
    int first, last;
    
    Person* *reprarray;
    void allocate(void);
    void deallocate(void);
};

/**
 * DISCRIPTION:
 * 1. Each Fib_node has value field "value" (which stores the key) and "degree" (which stores the number of children of this node), 
 *    and 4 pointer field (points to its left node, right node on the same level; points to its parent (if have), and points to its leftmost child (if have))
 * 
 * 2. The root list is a doubly circular linked list without dummy node (The "min" pointer is the only way to access the rootlist and all other elements in the fibonacci heap)
 * 
 * 3. The children of a node is connected by the left and right pointer, and this is also a circular linked list (The "left_most_child" is the only way to access the children of a node)
 * 
 */

class Fib_node
{
public:
    // Basic constructor, for declaration examples, see "fibonacci_test.cpp".
    Fib_node(Person *per) : value(per), left(NULL), right(NULL), left_most_child(NULL), parent(NULL), degree(0), mark(false) {};
    Person *value; // store the key value of the node
    Fib_node *left;               // pointer to its left node
    Fib_node *right;              // pointer to its right node
    Fib_node *left_most_child;    // pointer to its leftmost child (if have)
    Fib_node *parent;             // pointer to its parent (if have)
    
    int degree;   // The number of children of the node

    // The node is marked iff it has lost a child after it has been set as a child of its parent node 
    bool mark;
    // Information copying (when perform insertion and merging, the new inserted element will "gone away" after exiting the function, which could cause segmentation fault)
    void copy_node(Fib_node *old);
};

/**
 * DESCRIPTION:
 * 1. Two heap property is maintain permanently (the min pointer of the heap (NULL if the heap is empty), and the number of nodes in the fibonacci heap "numitems")
 * 
 * 2. The 6 heap methods and their helper functions (see concrete description in "fibonacci.cpp"):
 *        void insert(T value);
 *              void insert_node(Fib_node<T> *elem);
 * 
 *        Fib_node<T> *minimal(void);
 * 
 *        Fib_node<T> *extract_min(void);
 *              void cut(Fib_node<T> *node, Fib_node<T> *parent);
 *              Fib_node<T> *delete_node_root(Fib_node<T> *elem);
 *              void consolidate(void);
 * 
 *        void mergeheap(Fib_heap *H);
 *              void copy_node(Fib_node<T> *old);
 *              Fib_node<T> *delete_node_root(Fib_node<T> *elem);
 *              void insert_node(Fib_node<T> *elem);
 * 
 *        void decrease_key(Fib_node<T> *node, T k);    
 *              void cut(Fib_node<T> *node, Fib_node<T> *parent);
 *              void cascading_cut(Fib_node<T> *parent);
 * 
 *        void deletenode(Fib_node<T> *node);
 *              void decrease_key(Fib_node<T> *node, T k);
 *              Fib_node<T> *extract_min(void);
 * 
 * 3. Functions for dubugging purpose (see concrete description in "fibonacci.cpp"):
 *        void print_rootlist(void);
 *        void print_children(Fib_node<T> *parent);
 *        void print_parent(Fib_node<T> *child);
 *        void print_heap();
 *        void print_minimal();
 */

class Fib_heap
{
public:
    
    Fib_node *min;    // min pointer of the heap (NULL if the heap is empty)
    int numitems;        // the number of nodes in the fibonacci heap

    // Basic constructor, for declaration examples, see "fibonacci_test.cpp".
    Fib_heap() : min(NULL), numitems(0) {};

    // Helper function for "insert"
    void insert_node(Fib_node *elem);
    // Insert "value" into the fibonacci heap
    void insert(Person *value);
    // Return the pointer to the minimal node witout deleting this element
    Fib_node *minimal(void);
    // Return the pointer to the minimal node but the element has been deleted from the heap
    Fib_node *extract_min(void);
    // Merge two fibonacci heaps
    void mergeheap(Fib_heap *H);
    // Decrease the value of "node" to "k"
    void decrease_key(Fib_node *node, Person *k);
    // Adjust the decreased node
    void adjust_priority(Fib_node *node);
    // Delete node "node" in the fibonacci heap
    void deletenode(Fib_node *node);
    // Helper function
    void consolidate(void);
    // Cut the sub_heap rooted by "node" and reinsert in the rootlist
    void cut(Fib_node *node, Fib_node *parent);
    // Helper function 
    void cascading_cut(Fib_node *parent);
    // Delete "elem" in the rootlist (assume there is)
    Fib_node *delete_node_root(Fib_node *elem);
    // make "child" a child of "parent"
    void make_child(Fib_node *parent, Fib_node *child);
    // Delete "child" whose parent is "parent"
    void delete_child(Fib_node *parent, Fib_node *child);
    // print all the elements in the root list
    void print_rootlist(void);
    // print all the children of "parent"
    void print_children(Fib_node *parent);
    // print the parent of "child"
    void print_parent(Fib_node *child);
    // print all the elements in the heap
    void print_heap();
    // print the minimal element in the heap
    void print_minimal();
};

class Hashset
{
public:
    Hashset(int size = 20);

    // The operation add inserts an element into the hashtable, remove deletes an element and member checks, if the element is in the hashtable
    void add(Person* person);
    void remove(int id);
    Person* member(int id);
    // The function display is only used for testing purposes. It shows the size of the hashtable, the number of elements, and displays all entries
    void display(void);
private:
    int numitems;           // contains the number of elements in the hashtable
    Person **reprarray;
/* reprarray defines a pointer to (an array of) pointers to values of type T. We will use a null pointer, if an entry is undefined, and a fixed pointer as placeholder for free entries (after remove) */
    Person *pt_nil;              // used as placeholder
    int maxsize;            // defines the size of the hashtable
    void rehash(int newsize);
// rehash is an internal function to enlarge or shrink the hashtable,
// if the fill factors 0.75 or 0.25 are exceeded or undershot, resp.
};



// local registration system definition
// update person in the local queue
class Local_Registration_Queue          
{
public:
    Local_Registration_Queue();
    // Read information from the Database
    void add_person(Person *person); 
    // Withdraw person
    void delete_person(Person *person);
    // pop out person from the local queue
    Person *pop_person(void);
    // used for visualization and debugging
    void display_queue(void);

    void adjust_risk (Person *person, int new_risk); // Adjust the local queue when someone's risk status has changed.
    void adjust_pro (Person *person, int new_profession); // Adjust the local queue when someone's profession category has changed.
    void set_ddl (Person *person, int deadline); // Adjust the local queue when someone's treatment deadline has changed.
    // How many people are waiting now in the current queue?
    int num_of_people_waiting;

    fifo *local_queue;
};


// A set of local queues, forward the information to the central queue
class Local_set{
public:
    Local_set();
    void local_selection(Time *time);   // Send all people in local queues to central queue in a certain period.
    int num_of_local_queues;

    vector<Local_Registration_Queue*> *local_queues;
    vector<Person*> *people_selected; // People to be sent.
};



// Recieve person from central queue and allocate appointment time and date for that person
class Medical_center
{
friend class Central_Queue;
public:
    Medical_center(int num_hospital, vector<pair<int, int>> *locations);
    ~Medical_center();
    void receive (Person *person, int index, Date *today); // Recieve people sent from the centralized queue and assign them to the nearest location.
    void delete_person(Person *person); // Delete people who withdraw after having been assigned an appointment date. Though, the time slots allocated to this person would not be allocated again.
    bool is_free(int index, Person *person);
    bool capacity_free(int index);
    bool time_free(int index, Person *person);
    void treatment_complete(Time* today);
    void daily_update(void);

    int num_locations;
    //int capacity;
    vector<pair<int, int>> *address_location; // An array that stores the addresses of the treatment locations
    vector<vector<Person*>> *location_lists; // needs daily update
    vector<int> *location_ca; // needs daily update, capacity <=80
    // first int indicates total patient
    vector<vector<pair<int, int>>> *location_time; // needs daily update
    // first int in the vector indicates number of patients(at most 10)
    // second int in the vector indicates time slots 8:00-12:00 1:00-5:00 (1-8) 1 indicates 8:00a.m. ... 8 indicates 4:00p.m.
    //for no or low risk patient, 1 time slot needed, medium/high occupy 2. 
    
};

// Receive person from a set of local registry queues
class Central_Queue
{
public:
    Central_Queue();
    // randomly add local registries into the central queue by sending the whole set and choosing randomly.
    // add the ramdomly selected person into the fibonacci heap
    void add_person (Local_set *local_set); 

    // assign an appointment time for the person with the highest priority
    int send (Person *person, Medical_center *medctr, Time *today); // Send the selected person to medical center in priority. The priority of every person is determind in Fib, including consider 1. the added one month of people with middium risk, 2. people with high risk, 3. people who register again.
    void send_severe (Medical_center *medctr, Time *today); // Send those with ddl due today
    int send_norm_from_heap (Medical_center *medctr, Time *today); // Send the minimum in the heap

    void delete_person(Person *person); // Delete people who withdraw.
    void delete_severe(Person *person); //Delete those (with ddl) who withdraw.

    void adjust_attr (Person *person); // Adjust the Fib when someone's risk status/ professional category has changed.
    
    void find_severe(Person *person); // Adjust the Fib when someone's treatment deadline has changed.
    
    void add_severe(Person *person); // Person with ddl should be placed in the vector
    void print_rootlist(void);
    static bool sort_ddl(Person *first, Person *second){
        return (!(first->deadline->operator>(second->deadline)));
    }

    // Return the pointer to the node with value k
    Fib_node *search_fibheap(Fib_node *tempmin, Person *k);
    int confirm(Person *node_to_check, int root_num, int depth, int column);

    Fib_heap *Center_fib_heap;
    vector<Person*> *severe_per; // for those with ddls
};

/** All person's information is stored in the database, no matter what the status (applied, waiting, withdrawn, treated) a person is.
 *  ATTIBUTES INCLUDED IN THE INPUT_FILE:
 *      id_number, name, address, phone, wechat, email, profession_category, age, birth_date, risk level, time and date of registration
 *  NOT INCLUDED IN THE INPUT_FILE:
 *      age_category, status, appointment time and location, deadline of treatment
 */ 
class Person_Database
{
public:
    Person_Database() {
        collection = new vector<Person*>(0, NULL);
        database = new Hashset(1000);
    }
    // Functions for reporting
    // print report, UNTIL (year, month, day)
    // three boolean variables indicates the sorting criteria (AT MOST ONE "true" IS ALLOWED)
    void report_treated(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);
    void report_with_appointment(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);
    void report_without_appointment(Time *time, int past_week, bool by_name, bool by_profession, bool by_age);
    // print how many people have registered, are waiting, etc
    void monthly_report(Time *now, int past_month, Local_set *localset);
    // Add people to the database from file
    // And assign them into the local registry queue
    void register_file(const char filename[][51], int file_num, Local_set *all_queue);
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

};
void input_generator(int local_order, int num, Hashset* database);
#endif