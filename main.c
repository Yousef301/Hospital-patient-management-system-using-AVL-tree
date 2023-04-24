// By Yousef Shamasneh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//Define and initialize some variables that we need.
#define LoadingFactor 0.5
int sameIllness = 0; //count number of people with same illness.
int g_cnt = 0; //use to calculate loading factor
int SIZE = 13; //the initial size of hash table

typedef struct node {
    char pName[50];
    char gen;
    int aDate;
    int bDate;
    char illness[50];
    char address[50];
    char bType[50];
    int height;
    struct node *left;
    struct node *right;
} node;

node *root = NULL;

typedef struct hash {
    char pName[50];
    char gen;
    int aDate;
    int bDate;
    char illness[50];
    char address[50];
    char bType[50];
    char status[10];
} hash;

hash emptyNode;

hash *Rehashing(hash *);

void printPatientAVL(node *);

//find the height of node.
int height(node *tree) {
    int left, right;
    if (tree == NULL)
        return -1;
    left = height(tree->left);
    right = height(tree->right);

    if (left > right) return left + 1;
    return right + 1;
}

//Find the balance factor of a node.
int BF(node *tree) {
    int left, right;
    if (tree == NULL)
        return -1;
    left = height(tree->left);
    right = height(tree->right);
    return left - right;
}

//doing a left left rotate
node *LLRot(node *tree) {
    //initialize to pointer to node one point to the left of tree and the other to the right of the left
    node *l = tree->left;
    node *lR = l->right;

    //make the right of the left node point to the root node and the make the left of the root node point to the right of
    // the left node which it now became the root of the subtree or tree.
    l->right = tree;
    tree->left = lR;

    tree->height = height(tree); //Recalculate the heihgt of the node;
    l->height = height(l);

    if (root == tree)
        root = l;

    return l;
}

// All of the following rotate function are similar to the upper one but with a little difference.
node *LRRot(node *tree) {
    node *l = tree->left;
    node *lR = l->right;

    l->right = lR->left;
    tree->left = lR->right;

    lR->left = l;
    lR->right = tree;

    tree->height = height(tree);
    l->height = height(l);
    lR->height = height(lR);

    if (root == tree)
        root = lR;

    return lR;
}

node *RRRot(node *tree) {
    node *r = tree->right;
    node *rl = r->left;

    r->left = tree;
    tree->right = rl;

    tree->height = height(tree);
    r->height = height(r);

    if (root == tree)
        root = r;

    return r;
}

node *RLRot(node *tree) {
    node *r = tree->right;
    node *rl = r->left;

    r->left = rl->right;
    tree->right = rl->left;

    rl->right = r;
    rl->left = tree;

    tree->height = height(tree);
    r->height = height(r);
    rl->height = height(rl);

    if (root == tree)
        root = rl;

    return rl;
}

//calculate the next prime number of a given number and this function used to get the new size for the hash table
//before rehashing
int NextPrime(int num) {
    bool notPrime = false;
    for (int i = num + 1; i < 100; i++) {
        for (int j = 2; j < (int) i / 2; j++) {
            if (i % j == 0) {
                notPrime = true;
                break;
            }
        }
        if (notPrime == false) {
            return i;
        }
        notPrime = false;
    }
    return -1;
}

int sumOfASCII(const char *key) {
    int cnt = 0, sum = 0;
    while (key[cnt] != '\0') {
        if (key[cnt] == ' ')
            cnt++;
        sum += key[cnt];
        cnt++;
    }
    return sum;
}

int Index(int key) {
    return key % SIZE;
}

//handling with collision while inserting in the hash table.
int Probe(hash HSTable[], int index) {
    int index_2 = 0;
    while ((strcasecmp(HSTable[index_2 + index].status, "empty") != 0))
        index_2++;
    return (index_2 + index) % SIZE;
}

//Inserting node inside the AVL tree, with taking care of balance factor for each node (Save the tree balanced).
node *InsertNode(node *tree, char *pName, char gen, int aDate, int bDate, char *illness, char *address, char *bType) {
    if (tree == NULL) {
        node *temp = (node *) malloc(sizeof(node));
        strcpy(temp->pName, pName);
        temp->gen = gen;
        temp->aDate = aDate;
        temp->bDate = bDate;
        strcpy(temp->illness, illness);
        strcpy(temp->address, address);
        strcpy(temp->bType, bType);
        temp->height = 1;
        temp->right = temp->left = NULL;
        tree = temp;
    } else if (strcmp(pName, tree->pName) < 0)
        tree->left = InsertNode(tree->left, pName, gen, aDate, bDate, illness, address, bType);
    else if (strcmp(pName, tree->pName) > 0)
        tree->right = InsertNode(tree->right, pName, gen, aDate, bDate, illness, address, bType);
    else
        return tree;
    tree->height = height(tree);
    if (BF(tree) == 2 && BF(tree->left) == 1)
        return LLRot(tree);
    else if (BF(tree) == 2 && BF(tree->left) == -1)
        return LRRot(tree);
    else if (BF(tree) == -2 && BF(tree->right) == -1)
        return RRRot(tree);
    else if (BF(tree) == -2 && BF(tree->right) == 1)
        return RLRot(tree);
    return tree;
}


node *minValue(node *nodes) {
    node *temp = nodes;
    while (temp->left != NULL) temp = temp->left;
    return temp;
}

//read data from the file and save into the AVL tree.
node *SaveInAVL(node *tree, char *line) {
    char *name = strtok(line, "#");
    char *gen = strtok(NULL,
                       "#"); // when we pass NULL value, that mean that we ask to continue tokenizing the same thing as before (line).
    char *admissionDate = strtok(NULL, "#");
    char *birthDate = strtok(NULL, "#");
    char *illness = strtok(NULL, "#");
    char *address = strtok(NULL, "#");
    char *bloodType = strtok(NULL, "#");
    tree = InsertNode(tree, name, gen[0], atoi(admissionDate), atoi(birthDate), illness, address, bloodType);
    return tree;
}

//Insert patient into the given tree.
node *InsertPatient(node *tree) {
    char name[50], gender[10], illness[50], address[50], bloodType[5];
    int admissionDate, birthDate;
    printf("Patient Name:");
    scanf(" %[^\n]%*c", name);
    printf("Patient Gender. Insert (Male or Female) only:");
    scanf(" %[^\n]%*c", gender);
    while (strcasecmp(gender, "Male") != 0 && strcasecmp(gender, "Female") != 0) {
        printf("Reinsert the gender correctly please:");
        scanf("  %[^\n]%*c", gender);
    }
    printf("Birth date, as the following form (DDMMYYYY):");
    scanf(" %d", &birthDate);
    printf("Address:");
    scanf(" %[^\n]%*c", address);
    printf("Illness:");
    scanf(" %[^\n]%*c", illness);
    printf("Blood type:");
    scanf(" %[^\n]%*c", bloodType);
    printf("Admission date, as the following form (DDMMYYYY):");
    scanf("%d", &admissionDate);
    strcat(bloodType, "\n");
    strupr(gender);
    tree = InsertNode(tree, name, gender[0], admissionDate, birthDate, illness, address, bloodType);
    printf(" ---------------------------------\n");
    printf("|New patient inserted successfully|");
    printf("\n ---------------------------------\n\n");
    return tree;
}

//Delete a patient by searching for his name and then delete it.
node *Delete(node *tree, char *name) {
    if (tree == NULL) return tree;
    if (strcasecmp(name, tree->pName) < 0)
        tree->left = Delete(tree->left, name);
    else if (strcasecmp(name, tree->pName) > 0)
        tree->right = Delete(tree->right, name);
    else {
        if (tree->left == NULL) {
            node *temp = tree->right;
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            node *temp = tree->left;
            free(tree);
            return temp;
        } else {
            node *temp = minValue(tree->right);
            strcpy(tree->pName, temp->pName);
            temp->gen = tree->gen;
            temp->aDate = tree->aDate;
            temp->bDate = tree->bDate;
            strcpy(tree->bType, temp->bType);
            strcpy(tree->address, temp->address);
            strcpy(tree->illness, temp->illness);
            tree->right = Delete(tree->right, temp->pName);
        }
    }
    if (BF(tree) == 2 && BF(tree->left) == 1)
        return LLRot(tree);
    else if (BF(tree) == 2 && BF(tree->left) == -1)
        return LRRot(tree);
    else if (BF(tree) == -2 && BF(tree->right) == -1)
        return RRRot(tree);
    else if (BF(tree) == -2 && BF(tree->right) == 1)
        return RLRot(tree);
    return tree;
}

//this function used to search for a patient by his name and then give the choice for the user to update any
//of his info.
node *Search_Edit(node *tree, char *name) {
    node *temp = tree;
    if (temp == NULL) return tree;
    else {
        while (temp != NULL) {
            if (strcasecmp(name, temp->pName) < 0) {
                temp = temp->left;
            } else if (strcasecmp(name, temp->pName) > 0) {
                temp = temp->right;
            } else {
                printf(" ----------------\n");
                printf("|Patient is found|");
                printf("\n ----------------");
                printPatientAVL(temp);
                char names[50], gender[50], illness[50], address[50], bloodType[50];
                int admissionDate, birthDate, selection;
                while (1) {
                    printf("(1) Update patient name\n(2) Update patient gender\n(3) Update patient blood type\n(4) Update patient illness\n"
                           "(5) Update patient address\n(6) Update patient admission date\n(7) Update patient birth date\n(8) Exit");
                    printf("\nYour selection -->");
                    scanf("%d", &selection);
                    switch (selection) {
                        case 1:
                            printf("New name:");
                            scanf(" %[^\n]%*c", names);
                            tree = InsertNode(tree, names, temp->gen, temp->aDate, temp->bDate, temp->illness,
                                              temp->address,
                                              temp->bType);
                            tree = Delete(tree, temp->pName);
                            printf("Name has been successfully updated\n\n");
                            return tree;
                        case 2:
                            printf("Update gender. Insert (Male or Female) only::");
                            scanf(" %[^\n]%*c", gender);
                            while (strcasecmp(gender, "Male") != 0 && strcasecmp(gender, "Female") != 0) {
                                printf("Reinsert the gender correctly please:");
                                scanf(" %[^\n]%*c", gender);
                            }
                            temp->gen = gender[0];
                            printf("Gender has been successfully updated\n\n");
                            break;
                        case 3:
                            printf("Update blood type:");
                            scanf(" %[^\n]%*c", bloodType);
                            strcat(bloodType, "\n");
                            strcpy(temp->bType, bloodType);
                            printf("Blood type been successfully updated\n\n");
                            break;
                        case 4:
                            printf("Update patient illness:");
                            scanf(" %[^\n]%*c", illness);
                            strcpy(temp->illness, illness);
                            printf("Illness has been successfully updated\n\n");
                            break;
                        case 5:
                            printf("Update patient address:");
                            scanf(" %[^\n]%*c", address);
                            strcpy(temp->address, address);
                            printf("Address has been successfully updated\n\n");
                            break;
                        case 6:
                            printf("Update date of admission:");
                            scanf("%d", &admissionDate);
                            temp->aDate = admissionDate;
                            printf("Admission date has been successfully updated\n\n");
                            break;
                        case 7:
                            printf("Update date of birth:");
                            scanf("%d", &birthDate);
                            temp->bDate = birthDate;
                            printf("Birth date has been successfully updated\n\n");
                            break;
                        case 8:
                            return tree;
                        default:
                            printf(" ---------------\n");
                            printf("|Input invalid!!|");
                            printf("\n ---------------\n\n");
                    }
                }
            }
        }
        printf(" -----------------\n");
        printf("|Patient not found|");
        printf("\n -----------------\n\n");
    }
    return tree;
}

//list all patient with same illness in preorder.
void listByIllness(node *tree, char *illness) {
    if (tree == NULL) return;
    listByIllness(tree->left, illness);
    if (strcasecmp(illness, tree->illness) == 0) {
        printf("%s %c %d %d %s %s %s", tree->pName, tree->gen, tree->aDate, tree->bDate, tree->illness,
               tree->address,
               tree->bType);
        sameIllness++;
    }
    listByIllness(tree->right, illness);
}

//Save the data that's into the tree inside a file.
void writeInFile(node *tree, FILE *file) {
    if (tree == NULL) return;
    writeInFile(tree->left, file);
    fprintf(file, "%s#%c#%d#%d#%s#%s#%s", tree->pName, tree->gen, tree->aDate, tree->bDate, tree->illness,
            tree->address,
            tree->bType);
    writeInFile(tree->right, file);
}

//this function used inside hashing functions, it's make all given names have the same pattern like this (Xxxx Yyyy)
//first alphabet of each word are uppercase.
char *name_pattern(char *name) {
    int length = strlen(name);
    for (int i = 0; i < length; i++) {
        if (i == 0)
            name[0] = toupper(name[0]);
        else if (isspace(name[i - 1]) && isalpha(name[i]))
            name[i] = toupper(name[i]);
        else
            name[i] = tolower(name[i]);
    }
    return name;
}

hash *InsertInHSTable(hash *HSTable, char *name, char gen, int admissionDate, int birthDate, char *illness,
                      char *address, char *blood) {
    name = name_pattern(name);
    //before inserting we first check the loading factor if it's bigger than > 0.5 because if it's we need to rehash the hash table.
    double l_f = (double) g_cnt / SIZE;
    if (l_f < LoadingFactor) {
        int index, s_ascii;
        s_ascii = sumOfASCII(name);
        index = Index(s_ascii);
        if ((strcasecmp(HSTable[index].status, "empty") != 0) && (strcasecmp(HSTable[index].status, "deleted") != 0))
            index = Probe(HSTable, index);
        strcpy(HSTable[index].status, "booked up");
        strcpy(HSTable[index].pName, name);
        strcpy(HSTable[index].illness, illness);
        strcpy(HSTable[index].address, address);
        strcpy(HSTable[index].bType, blood);
        HSTable[index].bDate = birthDate;
        HSTable[index].aDate = admissionDate;
        HSTable[index].gen = gen;
        g_cnt++;
    } else {
        g_cnt = 0;
        HSTable = Rehashing(HSTable);
        int index, s_ascii;
        s_ascii = sumOfASCII(name);
        index = Index(s_ascii);
        if ((strcasecmp(HSTable[index].status, "empty") != 0) && (strcasecmp(HSTable[index].status, "deleted") != 0))
            index = Probe(HSTable, index);
        strcpy(HSTable[index].status, "booked up");
        strcpy(HSTable[index].pName, name);
        strcpy(HSTable[index].illness, illness);
        strcpy(HSTable[index].address, address);
        strcpy(HSTable[index].bType, blood);
        HSTable[index].bDate = birthDate;
        HSTable[index].aDate = admissionDate;
        HSTable[index].gen = gen;
        g_cnt++;
    }
    return HSTable;
}

hash *Rehashing(hash *oldTable) {
    int oldSize = SIZE;
    SIZE = NextPrime(SIZE * 2);
    hash *newTable = (hash *) malloc(SIZE * sizeof(hash)); // create the new hash table.
    for (int i = 0; i < SIZE; i++) {
        strcpy(newTable[i].status, "empty");
    }
    //Copy old HTable data to the new one, with taking care of the size of the new table.
    for (int i = 0; i < oldSize; i++) {
        if (strcasecmp(oldTable[i].status, "empty") == 0)
            continue;
        else {
            InsertInHSTable(newTable, oldTable[i].pName, oldTable[i].gen, oldTable[i].aDate, oldTable[i].bDate,
                            oldTable[i].illness, oldTable[i].address, oldTable[i].bType);
        }
    }
    free(oldTable);
    return newTable;
}

//Read all data from the AVL tree and save it into the hash table.
hash *SaveInHash(hash *table, node *tree) {
    node *current, *pre;
    if (tree == NULL)
        return NULL;
    current = tree;
    while (current != NULL) {
        if (current->left == NULL) {
            table = InsertInHSTable(table, current->pName, current->gen, current->aDate, current->bDate,
                                    current->illness,
                                    current->address, current->bType);
            current = current->right;
        } else {

            //Find the inorder predecessor of current
            pre = current->left;
            while (pre->right != NULL
                   && pre->right != current)
                pre = pre->right;

            // Make current as the right child of its inorder predecessor
            if (pre->right == NULL) {
                pre->right = current;
                current = current->left;
            } else {
                pre->right = NULL;
                table = InsertInHSTable(table, current->pName, current->gen, current->aDate, current->bDate,
                                        current->illness,
                                        current->address, current->bType);
                current = current->right;
            } // End of if condition pre->right == NULL
        } // End of if condition current->left == NULL
    }
    return table;
}


void DeleteFromHSTable(hash *HTable, char *name) {
    name = name_pattern(name);
    int key = sumOfASCII(name);
    int index = Index(key);
    while ((strcasecmp(name, HTable[index].pName) != 0)) { // Searching for the patient
        if (strcasecmp("empty", HTable[index].status) ==
            0) { // If there is an empty slot that mean patient won't be exist
            printf("Patient not found.");
            return;
        } else
            index++;
    }
    HTable[index] = emptyNode; //if patient found all of his data will be empty and his status will be changed to (deleted).
    strcpy(HTable[index].status, "deleted");
    g_cnt--;
}

int
SearchInHSTable(hash *HTable, char *name) { // searching for a patient, it's the similar to delete ope but not exact.
    name = name_pattern(name);
    if (HTable != NULL) {
        int key = sumOfASCII(name);
        int index = Index(key);
        while ((strcasecmp(HTable[index].pName, name) != 0)) {
            if (strcasecmp(HTable[index].status, "empty") == 0)
                return -1;
            else
                index++;
        }
        return index;
    } else
        return -1;
}

hash *InsertPatientInHash(hash *patient) {
    char name[50], gender[10], illness[50], address[50], bloodType[5];
    int admissionDate, birthDate;
    printf("Patient Name:");
    scanf(" %[^\n]%*c", name);
    printf("Patient Gender. Insert (Male or Female) only:");
    scanf(" %[^\n]%*c", gender);
    while (strcasecmp(gender, "Male") != 0 && strcasecmp(gender, "Female") != 0) {
        printf("Reinsert the gender correctly please:");
        scanf(" %[^\n]%*c", gender);
    }
    printf("Birth date, as the following form (DDMMYYYY):");
    scanf("%d", &birthDate);
    printf("Address:");
    scanf(" %[^\n]%*c", address);
    printf("Illness:");
    scanf(" %[^\n]%*c", illness);
    printf("Blood type:");
    scanf(" %[^\n]%*c", bloodType);
    printf("Admission date, as the following form (DDMMYYYY):");
    scanf("%d", &admissionDate);
    strcat(bloodType, "\n");
    strupr(gender);
    patient = InsertInHSTable(patient, name, gender[0], admissionDate, birthDate, illness, address, bloodType);
    printf(" ---------------------------------\n");
    printf("|New patient inserted successfully|");
    printf("\n ---------------------------------\n\n");
    return patient;
}

//Print patients in preorder by using recursive function
void PrintLexicographicOrder(node *tree) {
    if (tree == NULL)
        return;
    PrintLexicographicOrder(tree->left);
    printf("Patient name:%s\nGender:%c\nBirthDate:%d\nAddress:%s\nIllness:%s\nBlood type:%sAdmission date:%d\n\n",
           tree->pName, tree->gen, tree->bDate, tree->address, tree->illness, tree->bType,
           tree->aDate);
    PrintLexicographicOrder(tree->right);
}

void printPatient(hash *patient) {
    printf("\nPatient name:%s\nGender:%c\nBirthDate:%d\nAddress:%s\nIllness:%s\nBlood type:%sAdmission date:%d\n\n",
           patient->pName, patient->gen, patient->bDate, patient->address, patient->illness, patient->bType,
           patient->aDate);
}

void printPatientAVL(node *patient) {
    printf("\nPatient name:%s\nGender:%c\nBirthDate:%d\nAddress:%s\nIllness:%s\nBlood type:%sAdmission date:%d\n\n",
           patient->pName, patient->gen, patient->bDate, patient->address, patient->illness, patient->bType,
           patient->aDate);
}

void mainMenu() {
    printf("(1) Read patients file and load the data into an AVL tree\n(2) Insert a new patient\n(3) Search for a patient and update "
           "his information\n(4) List all patients in lexicographic order\n(5) List all patients that have the same illness\n"
           "(6) Delete a patient\n(7) Save all data into a file\n(8) EXIT SYSTEM OR OPEN HASH MENU");
}

void HashMenu() {
    printf("(1) Print hashed table\n(2) Print table size\n(3) Print out the used hash function\n"
           "(4) Insert a new record into the hash table\n(5) Search for a specific patient\n(6) Delete a specific record"
           "\n(7) Save data back then exit");
}

//Everything is clear inside main function most of steps are using the uppper functions.
int main() {
    char line[200], name[50], ill[25], patient[50], loadHash;
    int selectionMain, selectionHash;
    hash *table = (hash *) malloc(SIZE * sizeof(hash));
    for (int i = 0; i < SIZE; i++) {
        strcpy(table[i].status, "empty");
    }
    FILE *data, *write;
    data = fopen("patients.txt", "r");
    write = fopen("patients_hash.data", "w");
    printf("Hospital Patient Management System\n");
    while (1) {
        printf(" --------\n");
        printf("|AVL MENU|");
        printf("\n --------\n");
        mainMenu();
        printf("\nEnter your selection:");
        scanf("%d", &selectionMain);
        switch (selectionMain) {
            case 1:
                if (data == NULL) {
                    printf("Check the file name or the direction please!\n");
                    exit(1);
                } else {
                    while (fgets(line, sizeof(line), data)) {
                        if (line[0] != '\n') {
                            root = SaveInAVL(root, line);
                        }
                    }
                    printf(" --------------------------------------------------\n");
                    printf("|Data has been loaded successfully into an AVL tree|");
                    printf("\n --------------------------------------------------\n\n");

                    fprintf(data, "\n");
                    fclose(data);
                }
                break;
            case 2:
                root = InsertPatient(root);
                break;
            case 3:
                if (root != NULL) {
                    printf("Enter the full name of the patient:");
                    scanf(" %[^\n]%*c", name);
                    root = Search_Edit(root, name);
                } else {
                    printf(" ----------------\n");
                    printf("|List is empty...|");
                    printf("\n ----------------\n\n");
                }
                break;
            case 4:
                if (root != NULL) {
                    printf("\n");
                    PrintLexicographicOrder(root);
                    printf("\n");
                } else {
                    printf(" ----------------\n");
                    printf("|List is empty...|");
                    printf("\n ----------------\n\n");
                }
                break;
            case 5:
                if (root != NULL) {
                    printf("Enter the name of the illness:");
                    scanf(" %[^\n]%*c", ill);
                    printf("\n");
                    listByIllness(root, ill);
                    printf("Number of people that infected with (%s) is (%d)\n", ill, sameIllness);
                    printf("\n");
                    sameIllness = 0;
                } else {
                    printf(" ----------------\n");
                    printf("|List is empty...|");
                    printf("\n ----------------\n\n");
                }
                break;
            case 6:
                if (root != NULL) {
                    printf("Enter the full name of the patient:");
                    scanf(" %[^\n]%*c", patient);
                    root = Delete(root, patient);
                    printf(" ------------------------\n");
                    printf("|Patient has been deleted|");
                    printf("\n ------------------------\n\n");
                } else {
                    printf(" ----------------\n");
                    printf("|List is empty...|");
                    printf("\n ----------------\n\n");
                }
                break;
            case 7:
                if (root != NULL) {
                    writeInFile(root, write);
                    printf(" -------------------\n");
                    printf("|Data has been saved|");
                    printf("\n -------------------\n\n");
                    fclose(write);
                } else {
                    printf(" ----------------\n");
                    printf("|List is empty...|");
                    printf("\n ----------------\n\n");
                }
                break;
            case 8:
                printf("\nLoad data into hash table and open hash menu before saving? (Y/N):");
                scanf(" %c", &loadHash);
                while (loadHash != 'y' && loadHash != 'Y' && loadHash != 'n' && loadHash != 'N') {
                    printf("Please chose between (Y/N) only: ");
                    scanf(" %c", &loadHash);
                }
                if ((loadHash == 'y' || loadHash == 'Y') && root != NULL) {
                    table = SaveInHash(table, root);
                    printf(" -----------------------------------------------------");
                    printf("|Data has been loaded successfully into the Hash table|");
                    printf("\n -----------------------------------------------------\n\n");
                    while (1) {
                        printf(" ---------\n");
                        printf("|HASH MENU|");
                        printf("\n ---------\n");
                        HashMenu();
                        printf("\nEnter your selection:");
                        scanf("%d", &selectionHash);
                        switch (selectionHash) {
                            case 1:
                                for (int i = 0; i < SIZE; i++) {
                                    printf("Index (%d) ", i);
                                    if (strcasecmp(table[i].status, "empty") != 0 &&
                                        strcasecmp(table[i].status, "deleted") != 0) {
                                        printf("%s for the patient %s\n", table[i].status, table[i].pName);
                                    } else
                                        printf("empty\n");
                                }
                                printf("\n");
                                break;
                            case 2:
                                printf(" -------------------------\n");
                                printf("|Hash table size is --> %d|", SIZE);
                                printf("\n -------------------------\n");

                                break;
                            case 3:
                                printf(" ---------------------------------\n");
                                printf("| h(key)' = (h(key) + index) %% %d |\n", SIZE);
                                printf("| Where h(key) = key %% %d         |", SIZE);
                                printf("\n ---------------------------------\n");
                                break;
                            case 4:
                                table = InsertPatientInHash(table);
                                break;
                            case 5:
                                printf("Enter the full patient name:");
                                scanf(" %[^\n]%*c", name);
                                int check = SearchInHSTable(table, name);
                                if (check != -1) {
                                    printf(" ----------------\n");
                                    printf("|Patient is found|");
                                    printf("\n ----------------");
                                    printPatient(&table[check]);
                                } else {
                                    printf(" -----------------\n");
                                    printf("|Patient not found|");
                                    printf("\n -----------------\n\n");
                                }
                                break;
                            case 6:
                                printf("Enter the full patient name:");
                                scanf(" %[^\n]%*c", name);
                                DeleteFromHSTable(table, name);
                                printf(" ------------------------\n");
                                printf("|Patient has been deleted|");
                                printf("\n ------------------------\n\n");
                                break;
                            case 7:
                                write = fopen("patients_hash.data", "w");
                                for (int i = 0; i < SIZE; i++) {
                                    if (strcasecmp(table[i].status, "empty") != 0 &&
                                        strcasecmp(table[i].status, "deleted") != 0) {
                                        fprintf(write, "%s#%c#%d#%d#%s#%s#%s", table[i].pName, table[i].gen,
                                                table[i].aDate,
                                                table[i].bDate, table[i].illness, table[i].address, table[i].bType);
                                    } else
                                        continue;
                                }
                                fclose(write);
                                printf(" -------------------\n");
                                printf("|Data has been saved|");
                                printf("\n -------------------\n");
                                printf(" -------------------------\n");
                                printf("|System has been shut down|");
                                printf("\n -------------------------\n\n");
                                exit(1);
                            default:
                                printf(" ---------------\n");
                                printf("|Input invalid!!|");
                                printf("\n ---------------\n\n");
                        }
                    }
                } else if ((loadHash == 'y' || loadHash == 'Y') && root == NULL) {
                    printf(" ---------------------------------\n");
                    printf("|Make sure you read the file first|");
                    printf("\n ---------------------------------\n\n");
                } else {
                    printf(" -------------------------\n");
                    printf("|System has been shut down|");
                    printf("\n -------------------------\n\n");
                    exit(1);
                }
                break;
            default:
                printf(" ---------------\n");
                printf("|Invalid input!!|");
                printf("\n ---------------\n\n");
        }
    }
}
