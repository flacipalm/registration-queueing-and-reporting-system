#include "CA1.h"
using namespace std;

Local_Registration_Queue::Local_Registration_Queue()
{
    num_of_people_waiting = 0;
    local_queue = new fifo(10);
}

// Add people into the local queue according to the location he / she registered

void Local_Registration_Queue::add_person(Person *person)
{
    local_queue->pushback(person);
    person->status = REGISTERED;   // Change the status to "applied".
}

// People who have sent a withdraw letter
void Local_Registration_Queue::delete_person(Person *person)
{
    local_queue->deletion(person);
    num_of_people_waiting--;
    person->status = WITHDRAWN;
}

// Person *Local_Registration_Queue::pop_person(void)
// {
//     // Send a person to central queue.
//     num_of_people_waiting--;
//     return local_queue->popfront();
//     // The status will be changed in centralized queue.
// }

// void Local_Registration_Queue::adjust_risk (Person *person, int new_risk)
// {
//     if(new_risk < person-> risk){   person->risk = new_risk; }
// }

// void Local_Registration_Queue::adjust_pro (Person *person, int new_pro)
// {
//     if(new_pro < person->professional_category){    person->professional_category = new_pro; }
// }

void Local_Registration_Queue::display_queue(void)
{
    cout << endl << endl;
    cout << "there are " << num_of_people_waiting << " people waiting in this local queue." << endl;
    cout << "They are:" << endl;
    for (int i = 1; i <= num_of_people_waiting; i++) {
        Person* person = (*local_queue)[i];
        cout << "id:" << person->identification_number << "   ";
        cout << "name: " << person->name << "   ";
        cout << "Registration time: " << person->date->month << "-" << person->date->day << "-" << person->date->year << " ";
        cout << person->date->hour << "-" << person->date->minute << "-" << person->date->second << endl;
    }
}