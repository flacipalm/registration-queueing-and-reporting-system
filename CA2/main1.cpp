#include "database.cpp"
#include "Person.cpp"
#include "central_queue.cpp"
#include "Input_generator.cpp"

#include <string>
#include <vector>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>

// PUT ALL CONSTANTS HERE
#define TIME_INTERVAL 12
#define START_YEAR 2019
#define START_MONTH 1
#define START_DAY 1
#define DAY 2
#define WEEK 14
#define MONTH 60

int main()
{
    //======================Clear the report folder (If there is one)======================
    string path = "./Report";
    if (access(path.c_str(), 0) == F_OK) {
        system("rm -rf ./Report");

    }
    //======================Create a report folder======================
    system("mkdir Report");
    system("mkdir Report/Weekly");
    system("mkdir Report/Weekly/Treated");
    system("mkdir Report/Weekly/With_Appointment");
    system("mkdir Report/Weekly/Without_Appointment");
    system("mkdir Report/Monthly");

    // system("touch ./Report/ok.txt");
    // system("touch ./Report/Weekly/okk.txt");
    //======================Create all data structure needed here!==================
    Person_Database *test_database = new Person_Database();
    Local_set *all_queue = new Local_set();
    Central_Queue *center_queue = new Central_Queue();
    // Create the medical center
    int num_of_hospitals = 10;
    vector<pair<int, int>> *hospital_location = new vector<pair<int, int>>(0, make_pair(0, 0));
    for (int i = 0; i < num_of_hospitals; i++) hospital_location->push_back(make_pair(5000 + 300 * i, 5000 + 300 * i));
    Medical_center *medical_center = new Medical_center(num_of_hospitals, hospital_location);


    //=================================Generate Input=============================
    /* No need for this program.*/
    // cout << "Generating test cases..." << endl;
    // int file_num = 1;
    // input_generator(0, 5000, test_database->database);
    // const char filename0[file_num][51] = {"./My_example/examplefile1.txt"};
    // test_database->register_file(filename0, file_num, all_queue);

    // input_generator(1, 5000, test_database->database);
    // const char filename1[file_num][51] = {"./My_example/examplefile2.txt"};
    // test_database->register_file(filename1, file_num, all_queue);

    // input_generator(2, 5000, test_database->database);
    // const char filename2[file_num][51] = {"./My_example/examplefile3.txt"};
    // test_database->register_file(filename2, file_num, all_queue);

    // input_generator(3, 5000, test_database->database);
    // const char filename3[file_num][51] = {"./My_example/examplefile4.txt"};
    // test_database->register_file(filename3, file_num, all_queue);

    // input_generator(4, 5000, test_database->database);
    // const char filename4[file_num][51] = {"./My_example/examplefile5.txt"};
    // test_database->register_file(filename4, file_num, all_queue);


    //=================================READ INFORMATION FROM FILE=======================
    int file_num = 5;
    // const char filename[file_num][51] = {"./My_example/examplefile1.txt",
    //                                      "./My_example/examplefile2.txt",
    //                                      "./My_example/examplefile3.txt",
    //                                      "./My_example/examplefile4.txt",
    //                                      "./My_example/examplefile5.txt"};

    const char filename[file_num][51] = {"./My_example/examplefile1.txt",
                                        "./My_example/examplefile2.txt",
                                        "./My_example/examplefile3.txt",
                                        "./My_example/examplefile4.txt",
                                        "./My_example/examplefile5.txt"};
    test_database->register_file(filename, file_num, all_queue);
    //=================Add person to the central queue=================*/
    // set start time (DO NOT MODIFIY)
    Time *default_start_time = new Time();
    default_start_time->year = START_YEAR; default_start_time->month = START_MONTH; default_start_time->day = START_DAY;
    
    // set time duration (DO NOT MODIFY)
    Time *duration = new Time();
    duration->hour = TIME_INTERVAL;

    // set end time (modify as you like)
    Time *end_time = new Time();
    end_time->year = 2020; end_time->month = 1; end_time->day = 1;

    // DO NOT NODIFY
    Time *today = default_start_time;
    int past = 0; // Record the past time. 12-hour as unit.
    int past_week = 0;
    int past_month = 0;
    int a_day = DAY;
    int a_week = WEEK; // One week euquals to 14 "pasts". 7 * 2.
    int a_month = MONTH; // One month equals to 60 "pasts". 30 * 2.


    //===============================Test for Changing (only for the examplefiles we provide)======================

    // The following id of changing is specially picked from the examplefiles we provide.
    // If you want to test the registration system with other examplefiles, the following ids should be changed manually.
    // The registration without all changing is called "Normal Case". 
    // On contrary, the registration without all changing is called "Changing Case". 

    // 1. Test "withdraw"
    // The result can be seen in the monthly report.

    // 1-1. Withdraw with re-registration on 2019-1-8-00-00-00.
    int id_num_withdraw1 = 1385807; 
    // His/Her registration time is 2019-1-1-00-00-00. 
    // Normal case: His/Her original appointment time is 2019-1-6-9-00-00.
    Time *set_withdraw1 = new Time(); // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_withdraw1->year = 2019; set_withdraw1->month = 1; set_withdraw1->day = 5; // He/She withdraws on 2019-1-5-00-00.
    // Changeing case: His/Her new appointment time is 2019-1-23-8-00-00.


    // 1-2. Withdraw without re-registration.
    int id_num_withdraw2 = 1592800; 
    // His/Her registration time is 2019-1-1-00-00-00. 
    // Normal case: His/Her original appointment time is 2019-1-19-13-00-00.
    Time *set_withdraw2 = new Time(); // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_withdraw2->year = 2019; set_withdraw2->month = 1; set_withdraw2->day = 9; // He/She withdraws on 2019-1-20-00-00.
    // Changeing case: Withdraw.


    // 2. Test "set_ddl"
    // The result can be seen in the according weekly treated report (this person re-registered at 1.8, so to view the result, you should comment the re-registration code)

    int id_num_ddl = 1979128;
    // His/Her registration time is 2019-1-1-00-00-00. 
    // Normal case: His/Her original appointment time is 2019-2-14-10-00-00.
    Time *ddl = new Time(); // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    ddl->year = 2019; ddl->month = 1; ddl->day = 8; // He/She's treatment due to 2019-1-8-00-00
    Time *set_ddl = new Time();  // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_ddl->year = 2019; set_ddl->month = 1; set_ddl->day = 6; // He/She offers a letter with ddl on 2019-1-6-00-00.
    // Changing case: His/Her new appointment time is 2019-1-8-8-00-00.
    
    // 3. Test "adjust_profession"
    int id_num_adj_pro = 1957469;
    // His/Her registration time is 2019-1-1-00-00-00.
    // Normal case: His/Her professional category is 7 and appointment time is 2019-2-3-13-00-00.
    Time *set_adj_pro = new Time(); // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_adj_pro->year = 2019; set_adj_pro->month = 1; set_adj_pro->day = 5; // He/She adjusts professional category on 2019-1-5-00-00-00.
    int new_prof_cat = 1; // Change professional category to 1;
    // Changing case: His/Her new appointment time is 2019-1-9-15-00-00.

    // 4. Test "adjust_risk"
    int id_num_adj_risk = 1198603;
    // His/Her registration time is 2019-1-1-00-00-00.
    // Normal case: His/Her risk status is 3 and appointment time is 2019-2-5-10-00-00.
    // Changing
    Time *set_adj_risk = new Time; // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_adj_risk->year = 2019; set_adj_risk->month = 1; set_adj_risk->day = 11; // He/She adjusts professional category on 2019-1-5-00-00-00.
    int new_risk = 0; // Change risk to 0;
    // Changing case: His/Her new appointment time is 2019-1-12-8-00-00.

    // 5. Test "re-registration"
    Time *set_reg = new Time(); // set->minute and set->sec should be 0. set->hour is either 0 or 12.
    set_reg->year = 2019; set_reg->month = 1; set_reg->day = 8; // Reregist on 2019-1-8-00-00-00.
    // Changing case: His new appointment time is 2019-2-18-15-00-00-00.
    



    //======================System Starts running!======================
    do {
        past++;
        
        // Because the time passed into local_selction is the end_time of one single "push" to the central queue
        // so add 12 hours here
        today = today->operator+(duration);
        
        // push all people registered within the past 12 hours
        all_queue->local_selection(today);
        center_queue->add_person(all_queue);

        //=========================Call utility functions here===============
        test_database->delete_withdrawn(id_num_withdraw1, today, set_withdraw1, center_queue, all_queue, medical_center); 
        test_database->delete_withdrawn(id_num_withdraw2, today, set_withdraw2, center_queue, all_queue, medical_center); 
        
        test_database->set_ddl(id_num_ddl, today, set_ddl, ddl, center_queue);
        
        test_database->adjust_pro(id_num_adj_pro, today, set_adj_pro, new_prof_cat, center_queue);

        test_database->adjust_risk(id_num_adj_risk, today, set_adj_risk, new_risk, center_queue);

        test_database->register_withdraw(id_num_withdraw1, today, set_reg, all_queue);

        // send person into the central queue once a day
        if (!(past % a_day)){
            cout << past/2 << " days is past." << endl;
            
            // first send the person with a deadline today
            center_queue->send_severe(medical_center, today);

            // send person in the fibonacci heap into medical center (if have) until the medical center is full today
            do {
                if (center_queue->Center_fib_heap->min == NULL){
                    break;
                }
            } while (center_queue->send_norm_from_heap(medical_center, today) == 1);

            // Mark these person as COMPLETED
            test_database->treatment_complete(today); // Check completed treatment.

            // Empty the medical center today
            medical_center->daily_update();
        }

        //=====================Weekly report=====================
        if (!(past % a_week)){
            // Weekly reports.
            cout << "Generating weekly report..." << endl;
            past_week++;
            test_database->report_treated(today, past_week, 0, 1, 0); // Sort by name by default.
            test_database->report_with_appointment(today, past_week, 0, 1, 0);
            test_database->report_without_appointment(today, past_week, 0, 1, 0);
        }

        // ====================MOnthly report===================
        if (!(past % a_month)){
            // Monthly report.
            cout << "Generating monthly report..." << endl;
            past_month++;
            test_database->monthly_report(today, past_month, all_queue);
        }

    } while (today->operator<=(end_time));
    
    // When the end_time arrives, the program terminated
    cout << endl << "Make vaccine available for everyone across the world and heal the world without magic!!!" << endl;
    return 0;
}