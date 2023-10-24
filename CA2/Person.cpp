#include "CA1.h"
#include <iostream>
using namespace std;

bool Person::operator>(Person *second)
{
    // NULL has the highest priority
    if (this == NULL || second == NULL) {
        if (this == NULL) return false;
        // cout<<"true 1"<<endl;
        // cout << "return from NULL" << endl;
        return true;
    }

    if(this->risk == 3 || second->risk == 3){
        if(!(this->risk == 3 && second->risk == 3)){
            if(this->risk == 3) {
                // cout<<"true 2"<<endl;
                // cout << "return from risk" << endl;
                return true;
            }
            
            else {
                // cout << "return from risk" << endl;
                return false;
            }
        }
    }

    Time *date1 = this->date;
    Time *date2 = second->date;

    Time *extension = new Time();
    extension->day += 14;
    if(this->risk == 2){ date1 = date1->operator+(extension); }
    if(second->risk == 2){ date2 = date2->operator+(extension); }

    if(this->professional_category < second->professional_category) return false;
    if(this->professional_category > second->professional_category) return true;
    
    if (this->age_cat < second->age_cat) return false;
    if (this->age_cat > second->age_cat) return true;
    
    if(date1->operator<=(date2)) return false;
    
    // cout<<"true 3"<<endl;
    // cout << "Return other cases" << endl;
    return true;

}

bool Person::operator==(Person *second)
{
    if(this->identification_number == second->identification_number){  return true; }
    else return false;
}

void Person::convert_age_to_cat()
{
    if(age <= 12){  age_cat = CHILDREN; }
    else if(age <= 18){ age_cat = ADOLESCENT; }
    else if(age <= 35){ age_cat = YOUNG_ADULT; }
    else if(age <= 50){ age_cat = ADULT;}
    else if(age <= 65){ age_cat = SENIOR; }
    else if(age <= 75){ age_cat = ELDERLY_PEOPLE; }
    else{ age_cat = OLD_PEOPLE; }
}
