#ifndef HEADERFILE_H
#define HEADERFILE_H

struct ListNode
{
  /*Check in time*/
  int hour;
  int min;
  int sec;

  int id;
  int painLevel;
  int age;

  struct ListNode* next;
};

/*Keeps track of current time*/
extern int currentHour;
extern int currentMin;

/*Keeps track of # of patients transfered between patientList and waitingList*/
extern int patientsTransfered;


/*Functions*/
struct ListNode* createPatient(int hour, int min, int sec, int id,
			       int painLevel, int age);
void updateTime();
int convertTime(char time[10], char type, int hour);
int listLength(struct ListNode* head);
float getWaitTime(int hour, int min, int sec);
float longestWaitTime(struct ListNode* waitingList);
void push(struct ListNode** waitingList, int hour, int min, int sec, int id,
	  int painLevel, int age);
void pop(struct ListNode** List);
void attendPatient(struct ListNode* patient);
void freeList(struct ListNode* head);
struct ListNode* updateWaitingList(struct ListNode* patientList,
				   struct ListNode* waitingList);
struct ListNode* getData();

#endif
