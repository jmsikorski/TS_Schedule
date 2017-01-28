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
    int thresh = 0;
    int l_start = 4;
    do {
      if (i >= (t[x].week[0].start + l_start)*2 && t[x].week[0].job[i] == 'P' && i < (t[x].week[0].stop-2)*2) {
        for(int z = 0; z < t[x].week[0].lunch*2; z++) {
          if (s.total_diff[i+z] > thresh) {
            if(t[x].skill == 'C') {
              if(s.comm_diff[i+z] > thresh)
                ok = 1;
              else
                ok = 0;
              }
            else if(t[x].skill == 'R') {
              if(s.res_diff[i+z] > thresh)
                ok = 1;
              else
                ok = 0;
              }
            else if(t[x].skill == 'L') {
              if(s.led_diff[i+z] > thresh) {
                ok = 1;
                cout << i/2 << endl;
                cout << "next diff: " << s.led_diff[i+z+1] << "th: " << thresh << endl;
              }
              else {
                ok = 0;
                cout << t[x].lname << endl;
              }
            }
            else
              ok = 1;
          }
          else
            ok = 0;
        }
        if(ok == 1)
        {
          t[x].week[0].job[i] = 'L';
          t[x].week[0].lunch-= 0.5;
          s.total_avail[i]--;
          s.total_diff[i]--;
          if(t[x].skill == 'C') {
            s.comm_avail[i]--;
            s.comm_diff[i]--;
          }
          else if(t[x].skill == 'R') {
            s.res_avail[i]--;
            s.res_diff[i]--;
          }
          else if(t[x].skill == 'L') {
            s.led_avail[i]--;
            s.led_diff[i]--;
          }
        }
      }
      i++;
      if(i > 48 && l_start > 2) {
        l_start--;
        i = 0;
      }
      if(i > 48 && l_start == 2) {
        cout << "ERROR: Could not schedule " << t[x].lname << " lunch at " << thresh << " thresh" << endl;
        thresh--;
        l_start = 4;
        i = 0;
      }
      if(thresh == -5) {
        t[x].week[0].lunch = 0;
      }
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
    char skill;
    for(int i = 0; i < 48; i++)
      cin >> week[0].total_needs[i];
    getline(cin, name);
    for(int i = 0; i < 48; i++)
      cin >> week[0].comm_needs[i];
    getline(cin, name);
    for(int i = 0; i < 48; i++)
      cin >> week[0].res_needs[i];
    getline(cin, name);
    for(int i = 0; i < 48; i++)
      cin >> week[0].led_needs[i];
    getline(cin, name);
    while(getline (cin, name)) {
      table[count].fname = name;
      name_len += name.length();
      getline (cin, name);
      name_len += name.length();
      name_len++;
      table[count].lname = name;
      cin >> skill >> start >> finish >> email >> lunch >> c_start >> c_hours;
      table[count].skill = skill;
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
        if(table[i].week[0].job[x] == 'P') {
          week[0].total_avail[x]++;
          if(table[i].skill == 'C')
            week[0].comm_avail[x]++;
          else if(table[i].skill == 'R')
            week[0].res_avail[x]++;
          else if(table[i].skill == 'L')
            week[0].led_avail[x]++;
        }
      }
    }

    for(int i = 0; i < 48; i++) {
      week[0].total_diff[i] = week[0].total_avail[i] - week[0].total_needs[i];
      week[0].comm_diff[i] = week[0].comm_avail[i] - week[0].comm_needs[i];
      week[0].res_diff[i] = week[0].res_avail[i] - week[0].res_needs[i];
      week[0].led_diff[i] = week[0].led_avail[i] - week[0].led_needs[i];
    }
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
      if(week[0].total_avail[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Total Diff";
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

    cout << "Comm Needed";
    for(int x = 0; x < len-11; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].comm_needs[i];
      if(week[0].comm_needs[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Comm Agents";
    for(int x = 0; x < len-11; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].comm_avail[i];
      if(week[0].comm_avail[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Comm Diff";
    for(int x = 0; x < len-9; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].comm_diff[i];
      if(week[0].comm_diff[i] >= 10 || week[0].comm_diff[i] < 0)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Resi Needed";
    for(int x = 0; x < len-11; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].res_needs[i];
      if(week[0].res_needs[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Resi Agents";
    for(int x = 0; x < len-11; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].res_avail[i];
      if(week[0].res_avail[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "Resi Diff";
    for(int x = 0; x < len-9; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].res_diff[i];
      if(week[0].res_diff[i] >= 10 || week[0].res_diff[i] < 0)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "LED Needed";
    for(int x = 0; x < len-10; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].led_needs[i];
      if(week[0].led_needs[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "LED Agents";
    for(int x = 0; x < len-10; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].led_avail[i];
      if(week[0].led_avail[i] >= 10)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    cout << "LED Diff";
    for(int x = 0; x < len-8; x++)
      cout << " ";
    cout << "|";
    for(int i = 0; i < 48; i++) {
      cout << "  ";
      cout << week[0].led_diff[i];
      if(week[0].led_diff[i] >= 10 || week[0].led_diff[i] < 0)
        cout << " |";
      else
        cout << "  |";
    }
    cout << endl;

    for(int i = 0; i < count; i++)
      print_line(table[i], 0, len);
    return 0;

}
