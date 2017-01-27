/*
 * File:   schedule.cpp
 * Author: jmsikorski
 *
 * Created on January 23, 2017, 6:57 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include "support.h"

using namespace std;

int main(int argc, char** argv) {

    Employee table[100];
    table[0].fname = "Jason";
    table[0].lname = "Sikorski";
    Chat c;
    Meet m;
    Shift monday(2, 10, 0, c, m);
    Shift tuesday;
    table[0].week[0] = monday;
    table[0].week[1] = tuesday;
    cout << "Name           |";
    double x = 0.0;
    for(int i = 0; i < 48; i++) {
      cout << setprecision(2) << x << "|";
      x += 0.5;
    }
    cout << endl;
    cout << table[0].fname + " " + table[0].lname + ":";
    for(int i = 0; i < 48; i++) {
      cout << "| " << table[0].week[0].job[i] << " |";
    }
    cout << endl;
    cout << table[0].fname + " " + table[0].lname + ":";
    for(int i = 0; i < 48; i++) {
      cout << "| " << table[0].week[1].job[i] << " |";
    }
    cout << endl;

    cout << "test" << endl;
    return 0;

}
