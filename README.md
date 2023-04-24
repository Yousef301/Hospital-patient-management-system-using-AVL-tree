# Hospital patient management system using AVL tree
You will build a system to manage patients’ data in a hospital. The hospital patient management system stores specific information in the form of health record to keep track of the patients’ data.
Your program should read the information from a file called “patients.txt” that should be on the following format: Patient Name#Gender#Date of admission#Date of birth #Illness#Address (City)#Blood type

Example of data input:

Abed Mukhles#M#2212019#01012000#Ear Infection#Jenin#O+

Nadia A. Ali#F#01102020#05101970#COVID-19#AlBireh#A-

You are required to implement a program to help creating the system by including the following operations of an AVL tree:
  
    • Read the file patients.txt and load the data (i.e., AVL Tree).
  
    • Insert a new patient from user with all its associated data.
  
    • Find a patient and give the user the option to update the information of the patient if found.
  
    • List all patients in lexicographic order with their associated information.
  
    • List all patients that have the same illness.
  
    • Delete a patient from the system.
    
    • Save all words in file “patients_hash.data”
  
Then create a Hash Table using the patients’ data of the previous step (use names as keys). The project should use open addressing methods for collision resolution and implement the following functions on the hash table:
  
  • Print hashed table (i.e., print the entire table to the screen including empty spots).
  
  • Print out table size.
  
  • Print out the used hash function.
  
  • Insert a new record into the hash table.
  
  • Search for a specific patient.
  
  • Delete a specific record.
  
  • Save hash table back to file.
