#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#define  TWO_HOURS 120

/******************************************************************************
 *Creates a new Patient that points to NULL.
 *
 *Returns newly created patient. 
 *****************************************************************************/
struct ListNode* createPatient(int hour, int min, int sec, int id,
			       int painLevel, int age)
{
  /*Allocates space for new patient*/
  struct ListNode* patient = malloc(sizeof(struct ListNode));

  /*Sets Data*/
  patient->hour = hour;
  patient->min = min;
  patient->sec = sec;

  patient->id = id;
  patient->painLevel = painLevel;
  patient->age = age;

  patient->next = NULL;

  /*Returns data*/
  return patient;
}


/******************************************************************************
 *Updates current time (hour, min) by 15 minutes. Doesn't keep track of sec 
 * since we're updating by 15 whole minutes each time, seconds will never be 
 * 'updated'.
 * 
 *Returns nothing
 *****************************************************************************/
void updateTime()
{
  /*Adds +15 minutes if hour doesn't change (60 min = new hour mark)*/
  if(currentMin < 45)
    currentMin += 15;
  else
  {
    /*New hour mark*/
    currentHour += 1;
    
    /*New hour = reset minutes to 0*/
    currentMin = 0;
  }
}


/******************************************************************************
 *Breaks time string (raw input) into usable data (ints), depending on what 
 * part of time you want (hour = h, min = m, sec = s). When 'h' (hour) is 
 * passed to convert to int, a faulty hour point should be used. Anything 
 * will suffice, since it is not being checked.
 *
 *Returns either hour, min, or sec in 'int' data type
 *****************************************************************************/
int convertTime(char time[10], char type, int hour)
{
  /*Extracts Data from String*/
  int i;

  /*Holds tmp array for new value placement*/
  char tmp[3];
  
  /*Figures out what to return: Hour, minute, second*/
  switch(type)
  {
    case 'h':
      /*Converts to int*/
      i = atoi(time);

      break;
      
    case 'm':
      /*If Hour greater than 9hr, then position to read changes*/
      if(hour <= 9)
      {
	/*Pulls out info from middle of time array until end*/
	tmp[0] = time[2];
	tmp[1] = time[3];
      }
      else
      {
	/*Pulls out info from middle of time array until end*/
	tmp[0] = time[3];
	tmp[1] = time[4];
      }
      
      /*Converts minutes section to int*/
      i = atoi(tmp);
      
      break;
      
    case 's':
      /*If Hour greater than 9hr, then position to read changes*/
      if(hour <= 9)
      {
	/*Pulls out info from middle of time array until end*/
	tmp[0] = time[5];
	tmp[1] = time[6];
      }
      else
      {
	/*Pulls out info from middle of time array until end*/
	tmp[0] = time[6];
	tmp[1] = time[7];
      }
      
      /*Converts seconds section to int*/
      i = atoi(tmp);
      
      break;
      
    default:
      i = 0;
      printf("Time can't be parsed!\n");
  }

  /*Returns time*/
  return i;
}


/******************************************************************************
 *Gets length of a list. List may be waitingList or PatientList. 
 *
 *Returns length of list
 *****************************************************************************/
int listLength(struct ListNode* head)
{
  struct ListNode* current = head;
  int count = 0;

  while(current != NULL)
  {
    count++;
    current = current->next;
  }
  return count;
}


/******************************************************************************
 *Grabs a patient's check-in time and current time, and finds out how long a 
 * patient has been waiting for (in minutes). Converts their check-in time
 * (hour, min, sec) into a float number. Float number represents their waiting
 * time in minutes. Seconds are also accounted for (decimal), to compare
 * patients who entered at the same hour & min.
 *
 *Returns how many minutes a patient has been waiting for
 ******************************************************************************/
float getWaitTime(int hour, int min, int sec)
{ 
  float currentTime, waitingTime, checkinTime;
  
  /*Converts current time to minutes*/
  currentTime = (currentHour * 60) + currentMin;
    
  /*Converts patient's check-in time to minutes (seconds = decimal)*/
  checkinTime = ((hour * 60) + min) + (sec * .01);
    
  /*Subtract to get total wait time + Return*/
  return waitingTime = currentTime - checkinTime;;
}


/******************************************************************************
 *Iterates the waitingList and figures out how long the longest waiting 
 * patient has waited for (in minutes). Difference in getWaitTime() function
 * and longestWaitTime() is that this function is used to figure out if
 * we should go through the efforts to Iterate waitingList to find a patient
 * with the longest waitTime AND/OR to see which patient has been waiting 
 * the longest out of everyone.
 * 
 *Returns the longest waitTime in waitingList
 *****************************************************************************/
float longestWaitTime(struct ListNode* waitingList)
{
  /*Holds longest time in minutes*/
  float longestWait = 0;
  
  /*Variables*/
  float waiting;
  struct ListNode* patient = waitingList;

  /*Iterates waitingList (but NOT last faulty point)*/
  while(patient->next != NULL)
  {
    /*Converts hour, min, sec into minutes*/
    waiting = getWaitTime(patient->hour, patient->min, patient->sec);
    
    /*If waiting longer than previous patient, assign as longest waiter*/
    if(waiting > longestWait)
    {
      longestWait = waiting;
    }
    
    /*Next patient*/
    patient = patient->next;
  }
  
  /*Return longest wait time but in "Hours" NOT min*/
  return longestWait;
}



/******************************************************************************
 *Creates and Pushes patient into waitingList according to priority in 
 * Pain Level or age. 
 *
 *Returns nothing
 *****************************************************************************/
void push(struct ListNode** waitingList, int hour, int min, int sec, int id,
	                                 int painLevel, int age)
{
  struct ListNode* start = (*waitingList);
  
  /*Create new Patient*/
  struct ListNode* temp = createPatient(hour, min, sec, id, painLevel, age);
  
  /*Replaces head node if: head painLevel < new patient OR 
   *painLevel is same but head age is smaller than new patient.*/
  if((*waitingList)->painLevel < painLevel ||
     (start->painLevel == painLevel && start->age < age))
  {
    /*Insert new patient before head*/
    temp->next = *waitingList;
    (*waitingList) = temp;
  }
  else
  {
    /*Iterate List and find position for new patient*/
    while(start->next != NULL && start->next->painLevel >= painLevel)
    {
      /*If pain levels are same -> oldest = first (does NOT check head, 
       *head is check in 1st if-statement)*/
      if(start->next->painLevel == painLevel && start->next->age < age)
	break;
      
      /*Next Patient*/
      start = start->next;
    }
    
    /*Position was found, add*/
    temp->next = start->next;
    start->next = temp;
  }
}


/******************************************************************************
 *Function to remove element from list.
 *
 *Returns nothing
 *****************************************************************************/
void pop(struct ListNode** List)
{
  struct ListNode* temp = (*List);
  (*List) = (*List)->next;
  free(temp);
}


/******************************************************************************
 *Prints Patient information: current time, patient id, pain level, and how
 * long the patient has been waiting to be attended.
 *
 *Returns nothing
 *****************************************************************************/
void attendPatient(struct ListNode* patient)
{
  int checkinHour, checkinMin, checkinSec, hour, min, sec;
  
  /*Prints Time of Attending*/
  printf("%02d:%02d:00 ", currentHour, currentMin);

  /*Prints Patient ID*/
  printf("Patient: ID = %d, ", patient->id);

  /*Prints Patient Pain Level*/
  printf("Pain Level (1-10) = %d, ", patient->painLevel);

  /*Prints Patient Age*/
  printf("Age = %d years, ", patient->age);

  /*Waiting Time Logic*/
  checkinHour = patient->hour;
  checkinMin = patient->min;
  checkinSec = patient->sec;

  hour = 0;
  min = 0;
  sec = 0;
  
  /*Change currentMin to 60 if 0 (ex, 8:00)*/
  if(currentMin == 0)
    currentMin = 60;
  
  /*Sec Logic*/
  if(checkinSec != 0)
  {
    sec = 60 - checkinSec;
    checkinMin++;
  }
  /*Hour Logic*/
  while(checkinHour < currentHour - 1)
  {
    hour++;
    checkinHour++;
  }
  /*Min Logic*/
  if(currentMin >= checkinMin && currentHour == checkinHour)
    min = currentMin - checkinMin;
  else
  {
    /*Add until new hour is reached*/
    while(60 - checkinMin > 0)
    {
      min++;
      checkinMin++;
    }
    
    /*Convert 60 back to 0, for adding*/
    if(currentMin == 60)
      currentMin = 0;

    min = min + currentMin;
  }
  
  /*If minutes is over 60, separate to hour and min*/
  if(min >= 60)
  {
    hour++;
    min = min - 60;
  }
  
  /*Convert 60 back to 0, for currentMin*/
  if(currentMin == 60)
    currentMin = 0;
  
  /*Prints Waiting Time*/
  printf("Time waiting = ");
  printf("%02dh", hour);
  printf("%02dm", min);
  printf("%02ds", sec);
  printf("\n");
}


/******************************************************************************
 *Frees a list from memory
 *
 *Returns nothing
 *****************************************************************************/
void freeList(struct ListNode* head)
{
  struct ListNode* tmp;
  while(head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}


/******************************************************************************
 *Updates list of active patients. Iterates through patientList to find all
 * available patients. Patients will only be transfered to waitingList if 
 * patient checked-in before the current time. This function only updates
 * waiting list, does not modify patient list.
 *
 *Returns updated waiting list
 *****************************************************************************/
struct ListNode* updateWaitingList(struct ListNode* patientList,
				   struct ListNode* waitingList)
{
  struct ListNode* currentPatient;
  int id, age, painLevel, hour, min, sec;
  
  /*Update time to add new patients & reset counter*/
  updateTime();
  patientsTransfered = 0;
  
  
  currentPatient = patientList;
  
  
  while(currentPatient != NULL)
  {
    /*Variables*/
    hour = currentPatient->hour;
    min = currentPatient->min;
    sec = currentPatient->sec;
    id = currentPatient->id;
    painLevel = currentPatient->painLevel;
    age = currentPatient->age;
    
    
    /*Create new patient ONLY if patient came before current time*/
    if((currentHour == currentPatient->hour && currentMin > currentPatient->min)
       || (currentHour > currentPatient->hour))
    {
      /*Transfers Patient into waiting list*/
      push(&waitingList, hour, min, sec, id, painLevel, age);

      /*Keeps track of how many patients we pushed per batch*/
      patientsTransfered++;
      
      /*Next patient*/
      currentPatient = currentPatient->next;
    }
    else
      break;
  }

  /*Returning updated waiting list*/
  return waitingList;
}


/******************************************************************************
 *Gets ALL Data from File & calls createPatient() to make new patients. Gets
 * all data from reading a file and puts it into patientList. Data to be read
 * consist of id, time, age, and pain levels. 
 *
 *Returns patientList
 *****************************************************************************/
struct ListNode* getData()
{
  /*Variables*/
  char time[10], line[60];
  int id, age, painLevel, hour, min, sec;
  
  
  /*Faulty Start Point (starts a linked list)*/
  struct ListNode* patientList = createPatient(0, 0, 0, 0, 0, 0);
  
  
  /*Holds patient list*/
  struct ListNode* newPatient = NULL;
  struct ListNode* currentPatient = patientList;
  
  /*Grabs info from text file*/
  while(fgets(line, 60, stdin) != NULL)
  {
    /*Grabs 1 line (at a time)*/
    sscanf(line,"%d %s %d %d",&id, time, &age, &painLevel);
    
    /*Break time (string) up into usable data (int), hour has faulty point*/
    hour = convertTime(time, 'h', 0);
    min = convertTime(time, 'm', hour);
    sec = convertTime(time, 's', hour);
    
    /*Creates new patient*/
    newPatient = createPatient(hour, min, sec, id, painLevel, age);
    
    /*Add patient to list*/
    currentPatient->next = newPatient;
    currentPatient = newPatient;
  }
  
  return patientList;
}
