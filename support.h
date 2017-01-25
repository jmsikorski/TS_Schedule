/* 
 * File:   support.h
 * Author: jmsikorski
 *
 * Created on January 23, 2017, 7:20 PM
 */
using namespace std;

class Shift {
    string day;
    int start, stop, email;
    char job[48];
};

class Employee {
    string fname, lname, skill;
    Shift week[5]; 
    Train training;
    Meet meeting;
    Chat chating;
    char job[48];
public:
    void set_fname (string);
    void set_lname (string);
    void set_week (Shift)
};

class Train {
    string collegues[100];
    int hours;
};

class Meet {
    string collegues[100];
    int hours;
};

class Chat {
    int start, hours;
};

class Schedule {
    int comm_needs[48], res_needs[48], led_needs[48], total_needs[48];
    int comm_avail[48], res_avail[48], led_avail[48], total_avail[48];
    string day;
};
