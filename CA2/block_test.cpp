#include "CA1.h"
#include "CA2.h"
#include "Person.cpp"
#include "central_queue.cpp"
#include "Input_generator.cpp"
#include "Main_data.cpp"
#include "Block.cpp"

#include <cstdlib>
#include <dirent.h>
#include <unistd.h>

// test whether Main_data and blocks works well

// read files, record tuples in main_data, and display

int main()
{
    //======================Clear the report folder (If there is one)======================
    string path = "./Report";
    if (access(path.c_str(), 0) == F_OK) {
        system("rm -rf ./Report");

    }
    //======================Create a report folder======================
    system("mkdir Report");    


    // Generate input
    Person_Database database_test;

    input_generator(0, 557, database_test.database);
    
    // read file
    Main_data test_main_data = Main_data();

    int file_num = 1;

    Local_set *all_queue = new Local_set();
    
    const char filename[file_num][51] = {"./My_example/New_examplefile1.txt"};

    test_main_data.register_file(filename, file_num);


    return 0;
}