/******************************************************************************
 *Fermin Ramos
 *This program grabs input from a file and figures out the correct order to 
 * attend patients in an HR room. Program hands 2 linked lists: patientList &
 * waitingList. PatientList contains all patients from input file. WaitingList
 * contains all possible patients to be attended. WaitingList patients are
 * transfered from patientList if they check-in before the current time. I.e.
 * they're not from the future. WaitingList will ALWAYS have a faulty node,
 * represented by 0's in all categories. This is necessary to add patients
 * to list. WaitingList is organized by painLevel first, then age, and finally
 * time of arrival (if painLevel and age matches). Program constantly checks
 * if anybody has been waiting for longer than 2 hours. If patient waits for
 * more than 2 hrs, they are attended immediately after checking 10's
 * pain levels in waitingList.
 *
 *This program is part of a 3 file system: main.c, list.c, and header.h
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#define TWO_HOURS 120

/*Keeps track of Current Time*/
int currentHour = 7;
int currentMin = 30;

/*Keeps track of # of patients transfered between patientList and waitingList*/
int patientsTransfered;


int main()
{
  struct ListNode* patientList;
  struct ListNode* waitingList;
  float waitTime;
  int i;
  
  /*Gets Data from file and add to list of active Patients*/
  patientList = getData();
  
  /*Remove Faulty node (beginning of list)*/
  pop(&patientList);
    
  /*Creates waitingList, with a faulty point to add patient to*/
  waitingList = createPatient(0,0,0,0,0,0);

  /*Main Loop...Lops until no more patients left to attend OR on waitingList*/
  while(listLength(patientList) > 0 || listLength(waitingList) >= 1)
  {
    /*Updates WaitingList*/
    waitingList = updateWaitingList(patientList, waitingList);

    /*Remove all patients transfered to waitingList from our patientList*/
    for(i = 0; i < patientsTransfered; i++)
      pop(&patientList);

    /*Checks if any patient in waitingList has been waiting for +2hrs*/
    waitTime = longestWaitTime(waitingList);

    /*If painLevel is NOT 10 OR has been waiting for more than +2hrs*/
    if(waitingList->painLevel != 10 && waitTime >= TWO_HOURS)
    {
      struct ListNode* patient = waitingList;
      struct ListNode* prevPatient = waitingList;

      /*Find patient with longestWaitTime*/
      while(patient->next != NULL &&
	    getWaitTime(patient->hour, patient->min, patient->sec) != waitTime)
      {
	prevPatient = patient;
	patient = patient->next;
      }

      /*If head is being removed, replace head*/
      if(prevPatient->id == patient->id)
      {
	attendPatient(waitingList);
	pop(&waitingList);
      }
      else
      {
	/*head is not being removed, so remove from middle*/
	prevPatient->next = patient->next;
	patient->next = NULL;

	attendPatient(patient);
	free(patient);
      }
    }
    else
    {
      /*Nobody has been waiting for more than 2hrs, remove waitingList head*/
      if(listLength(waitingList) > 1)
      {
	attendPatient(waitingList);
	pop(&waitingList);
      }
	
    }

    /*Breaks from lop if there's nothing else left*/
    if (listLength(patientList) == 0 && listLength(waitingList) == 1)
      break;
  }

  /*Frees data*/
  freeList(patientList);
  freeList(waitingList);
  
  return 0;
}
