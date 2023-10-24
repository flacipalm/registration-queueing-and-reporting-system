#include "CA1.h"

Local_set::Local_set()
{
    num_of_local_queues = 0;
    local_queues = new vector<Local_Registration_Queue*>(0, NULL);
    people_selected = new vector<Person*>(0, NULL);
}

void Local_set::local_selection(Time *time)
{
    // Send all people in local queues to central queue in a certain period.
    Time *period = new Time();
    period->hour = 12;
    // cout << "local_Selction calls function operator-." << endl;
    Time *start = time->operator-(period);
    
    if (!people_selected->empty()) this->people_selected->clear();
    Person* temp;
    
    // Check point here
    // cout << "Start from: ";
    // cout << start->year << " " << start->month << " " << start->day << " " << start->hour << " " << start->minute << " " << start->second << endl;
    // cout << "End at: ";
    // cout << time->year << " " << time->month << " " << time->day << " " << time->hour << " " << time->minute << " " << time->second << endl;
    // cout << "Program runs here!" << endl;
    int queue_idx = 0;
    for(auto queue : *(this->local_queues)){

        int mark = 0;

        //index start from 1?
        // cout << "People selected from queue" << queue_idx << endl;
        int i = 1;
        for(; i <= queue->local_queue->getlength(); ++i){
            // cout << "infinite loop check" << endl;
            temp = (*(queue->local_queue))[i];
            bool cond_1 = temp->date->operator>(start), cond_2 = temp->date->operator==(start), cond_3 = !(temp->date->operator>(time)); // Not early than start time and no later than current time. 
            // cout << cond_1 << " " << cond_2 << " " << cond_3 << endl;
            if( ( cond_1 || cond_2 ) && cond_3 ){
                // cout << "Pushing!" << endl;
                this->people_selected->push_back(temp);
            }
            else{   
                mark = 1;
                i--;
                break; 
            } // alright? Maybe right.
        }

        if(mark == 0) { i--; }

        for(; i > 0; i--){
            // cout << "hhh:" << queue->local_queue->numitems << endl;
            queue->local_queue->popfront();
        }

        queue_idx++;
    }

}