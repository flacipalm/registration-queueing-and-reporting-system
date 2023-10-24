#include "CA2.h"

using namespace std;

// include other file when needed

class hash_chaining
{
public:
    hash_chaining() {
        hash_size = 56;
        hashset = vector<vector<Tuple>>(hash_size, vector<Tuple>(0)); 
    }
    vector<vector<Tuple>> hashset;
    int hash_size;
    int hash_func(Tuple person);
    void insert_tuple(Tuple person);
    void delete_tuple(Tuple person);
    void display();
};

int hash_chaining::hash_func(Tuple person)
{
    return person.pro + person.age + person.risk * 14;
}

void hash_chaining::insert_tuple(Tuple person)
{
    int pos = hash_func(person);
    // hashset[pos].push_back(person); // Wait...
    // loop from back
    int i;
    Tuple dummy = Tuple();
    dummy.id = 0;
    hashset[pos].push_back(dummy);

    for (i = hashset[pos].size() - 2; i >= 0; i--) 
    {
        if (hashset[pos][i].date->operator>(person.date)) hashset[pos][i+1] = hashset[pos][i];
        else break;
    }
    hashset[pos][i+1] = person;
}

void hash_chaining::delete_tuple(Tuple person)
{
    int pos = hash_func(person);
    int i;
    for (i = 0; i < hashset[pos].size(); i++)
    {
        if (hashset[pos][i].id == person.id) break;
    }
    hashset[pos].erase(hashset[pos].begin() + i);

}

void hash_chaining::display(){
    int number = 0;
    cout << "Generating report for hashtable..." << endl;
    ofstream fout;
    string filename = "Report/hashtable.txt";
    fout.open(filename);
    cout << "Secondary keys are age_category, profession_category, risk level and registration time." << endl;
	fout << "Secondary keys are age_category, profession_category, risk level and registration time." << endl << endl;
    for (int i = 0; i < hash_size; i++) {
        fout << "In entry" << i + 1 << endl;
        for (int j = 0; j < hashset[i].size(); j++){
            fout << " id: " << hashset[i][j].id << "   Registration time: " << hashset[i][j].date->year << "-" << hashset[i][j].date->month 
            << "-" << hashset[i][j].date->day << "-" << hashset[i][j].date->hour << "-" << hashset[i][j].date->minute << "-" << hashset[i][j].date->second << 
            "   Hash value: " << hash_func(hashset[i][j]) <<endl;
        }
        number += hashset[i].size();
        fout << "\n" <<endl;
    }
    fout << "There are " << number << " people in the hashtable." << endl;
    fout.close();
    cout << "There are " << number << " people in the hashtable."<< endl;
    cout << "Finished." << endl << endl;
}
