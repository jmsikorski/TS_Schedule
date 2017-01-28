/*
 * File:   support.h
 * Author: jmsikorski
 *
 * Created on January 23, 2017, 7:20 PM
 */

using namespace std;

class Train {
  public:
    string collegues[100];
    int hours;
};

class Meet {
  public:
    string collegues[100];
    int hours;
    Meet(string, int);
    Meet();
};

Meet::Meet(string c, int h) {
  collegues[0] = c;
  hours = h;
}

Meet::Meet() {
  collegues[0] = "none";
  hours = 0;
}

class Chat {
  public:
    double start, hours;
    Chat(double, double);
    Chat();
};

Chat::Chat(double s, double h) {
  start = s;
  hours = h;
}

Chat::Chat() {
  start = 0;
  hours = 0;
}

class Schedule {
  public:
    double comm_needs[48], res_needs[48], led_needs[48], total_needs[48];
    double comm_avail[48], res_avail[48], led_avail[48], total_avail[48];
    double comm_diff[48], res_diff[48], led_diff[48], total_diff[48];
    string day;
};

class Shift {
  public:
    string day;
    double start, stop, email, lunch;
    Chat chatting;
    Meet meeting;
    char job[48];
    Shift(double, double, double, double, Chat, Meet);
    Shift();
};

Shift::Shift(double s, double f, double e, double l, Chat c, Meet m) {
  start = s;
  stop = f;
  email = e;
  lunch = l;
  chatting = c;
  meeting = m;
  for(int i = 0; i < 48; i++) {
    if(i >= s*2 && i < f*2) {
      if(i >= chatting.start*2 && chatting.hours > 0) {
        job[i] = 'C';
        chatting.hours-=0.5;
      }
      else
        job[i] = 'P';
    }
    else {
      job[i] = ' ';
    }
  }
}

Shift::Shift() {
  start = 0;
  stop = 0;
  email = 0;
  lunch = 0;
  Chat c;
  chatting = c;
  Meet m;
  meeting = m;
  for (int i = 0; i < 48; i++)
    job[i] = ' ';
}

class Employee {
  public:
    string fname, lname;
    char skill;
    int id;
    Shift week[5];
    Train training;
};
