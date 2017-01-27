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

void print_header(int l){
  cout << "Name";
  for(int i = 0; i < l-4; i++)
    cout << " ";
  cout << "|";
  for(int i = 0; i < 24; i++) {
    if(i < 10)
      cout << "0" << i << ":00|0" << i << ":30|";
    else
      cout << i << ":00|" << i << ":30|";
  }
  cout << endl;
}

void print_line(Employee e, int d, int l){
  cout << e.fname + " " + e.lname;
  int len = e.fname.length() + e.lname.length() + 1;
  for(int i = 0; i < l-len; i++)
    cout << " ";
  for(int i = 0; i < 48; i++) {
    cout << "|  " << e.week[d].job[i] << "  ";
  }
  cout << "|" << endl;
}

void schedule_lunch(Employee t[], Schedule &s, int c) {
  for(int x = 0; x < c; x++){
    int ok = 0;
    int i = 0;
    do {
      if (i >= (t[x].week[0].start + 4)*2 && t[x].week[0].job[i] == 'P') {
        if (i > (t[x].week[0].start + 6)*2) {
          i = (t[x].week[0].start + 3)*2;
          do {
            if (i >= (t[x].week[0].start + 3)*2 && t[x].week[0].job[i] == 'P') {
              for(int z = 0; z < t[x].week[0].lunch; z++) {
                if (s.total_diff[i+z] > 0)
                  ok = 1;
                else
                  ok = 0;
                if(ok == 1)
                {
                  t[x].week[0].job[i] = 'L';
                  t[x].week[0].lunch-= 0.5;
                  s.total_avail[i]--;
                  s.total_diff[i]--;
                }
              }
            }
            else
              cout << "ERROR: Could not schedule " << t[x].lname << " lunch!" << endl;
            i++;
          } while (t[x].week[0].lunch > 0);
        }

        for(int z = 0; z < t[x].week[0].lunch; z++) {
          if (s.total_diff[i+z] > 0)
            ok = 1;
          else
            ok = 0;
          if(ok == 1)
          {
            t[x].week[0].job[i] = 'L';
            t[x].week[0].lunch-= 0.5;
            s.total_avail[i]--;
            s.total_diff[i]--;
          }
        }
      }
      i++;
    } while (t[x].week[0].lunch > 0);
  }
}

void schedule_email(Employee t[], Schedule &s, int c) {
  for(int x = 0; x < c; x++){
    int i = 0;
    if(t[x].week[0].email > 0) {
      do {
        if (t[x].week[0].job[i] == 'P') {
          if (s.total_diff[i] > 0) {
            t[x].week[0].job[i] = 'E';
            t[x].week[0].email-= 0.5;
            s.total_avail[i]--;
            s.total_diff[i]--;
          }
        }
      i++;
      } while (t[x].week[0].email > 0);
    }
  }
}

int main(int argc, char** argv) {

    Employee table[100];
    Schedule week[5];
    string name;
    double start, finish, email, lunch, c_start, c_hours;
    int count = 0;
    int len = 0;
    int name_len = 0;
    for(int i = 0; i < 48; i++)
      cin >> week[0].total_needs[i];
    getline(cin, name);
    while(getline (cin, name)) {
      table[count].fname = name;
      name_len += name.length();
      getline (cin, name);
      name_len += name.length();
      name_len++;
      table[count].lname = name;
      cin >> start >> finish >> email >> lunch >> c_start >> c_hours;
      Chat c(c_start, c_hours);
      Meet m;
      Shift monday(start, finish, email, lunch, c, m);
      table[count].week[0] = monday;
      count++;
      getline (cin, name);
      if(name_len > len)
        len = name_len;
      name_len = 0;
    }

    for(int i = 0; i < count; i++) {
      for(int x = 0; x < 48; x++) {
        if(table[i].week[0].job[x] == 'P')
          week[0].total_avail[x]++;
        }
    }

    for(int i = 0; i < 48; i++)
      week[0].total_diff[i] = week[0].total_avail[i] - week[0].total_needs[i];

    schedule_lunch(table, week[0], count);
    schedule_email(table, week[0], count);
    print_header(len);
    cout << "Total Needed";
    for(int x = 0; x < len-12; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].total_needs[i];
      if(week[0].total_needs[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Total Agents";
    for(int x = 0; x < len-12; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].total_avail[i];
      if(week[0].total_avail[i] > 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Difference";
    for(int x = 0; x < len-10; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].total_diff[i];
      if(week[0].total_diff[i] >= 10 || week[0].total_diff[i] < 0)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    for(int i = 0; i < count; i++)
      print_line(table[i], 0, len);
    return 0;

}
