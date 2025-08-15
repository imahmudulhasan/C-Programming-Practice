#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Structures =====
typedef struct Transaction {
    char type[10];
    float amount;
    struct Transaction* next;
} Transaction;

typedef struct Customer {
    int accNumber;
    char name[50];
    char password[20];
    float balance;
    Transaction* transactions;
    struct Customer* next;  // Linked List
    struct Customer* left;  // BST
    struct Customer* right; // BST
} Customer;

typedef struct QueueNode {
    int accNumber;
    struct QueueNode* next;
} QueueNode;

// ===== Global Variables =====
Customer* head = NULL;  // Linked List head
Customer* root = NULL;  // BST root
QueueNode* front = NULL;
QueueNode* rear = NULL;

// ===== Queue Functions =====
void enqueue(int acc) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->accNumber = acc;
    temp->next = NULL;
    if (rear == NULL) {
        front = rear = temp;
    } else {
        rear->next = temp;
        rear = temp;
    }
}

int dequeue() {
    if (front == NULL) {
        printf("Queue is empty.\n");
        return -1;
    }
    int acc = front->accNumber;
    QueueNode* temp = front;
    front = front->next;
    free(temp);
    if (front == NULL) rear = NULL;
    return acc;
}

// ===== Stack Functions =====
void pushTransaction(Transaction** top, char type[], float amount) {
    Transaction* newT = (Transaction*)malloc(sizeof(Transaction));
    strcpy(newT->type, type);
    newT->amount = amount;
    newT->next = *top;
    *top = newT;
}

void viewTransactions(Transaction* top) {
    if (!top) {
        printf("No transactions yet.\n");
        return;
    }
    printf("Transaction History:\n");
    while (top) {
        printf("%s: %.2f\n", top->type, top->amount);
        top = top->next;
    }
}

// ===== Linked List & BST Functions =====
Customer* createCustomer(int accNumber, char name[], char pass[], float balance) {
    Customer* newC = (Customer*)malloc(sizeof(Customer));
    newC->accNumber = accNumber;
    strcpy(newC->name, name);
    strcpy(newC->password, pass);
    newC->balance = balance;
    newC->transactions = NULL;
    newC->next = NULL;
    newC->left = newC->right = NULL;
    return newC;
}

void insertList(Customer* newC) {
    newC->next = head;
    head = newC;
}

Customer* insertBST(Customer* root, Customer* newC) {
    if (root == NULL) return newC;
    if (newC->accNumber < root->accNumber)
        root->left = insertBST(root->left, newC);
    else
        root->right = insertBST(root->right, newC);
    return root;
}

Customer* searchBST(Customer* root, int accNumber) {
    if (root == NULL || root->accNumber == accNumber) return root;
    if (accNumber < root->accNumber) return searchBST(root->left, accNumber);
    else return searchBST(root->right, accNumber);
}

void viewAllCustomers() {
    Customer* temp = head;
    printf("\n--- All Customers ---\n");
    while (temp) {
        printf("Account: %d | Name: %s | Balance: %.2f\n", temp->accNumber, temp->name, temp->balance);
        temp = temp->next;
    }
}

// ===== Menus =====
void bankOfficerMenu() {
    int choice, acc;
    char name[50], pass[20];
    float bal;
    while (1) {
        printf("\n--- Bank Officer Menu ---\n");
        printf("1. Add Customer\n2. Search Customer\n3. Enqueue Service\n4. Dequeue Service\n5. View All Customers\n0. Logout\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter Account Number: "); scanf("%d", &acc);
                printf("Enter Name: "); scanf("%s", name);
                printf("Enter Password: "); scanf("%s", pass);
                printf("Enter Initial Balance: "); scanf("%f", &bal);
                Customer* newC = createCustomer(acc, name, pass, bal);
                insertList(newC);
                root = insertBST(root, newC);
                printf("Customer added successfully.\n");
                break;
            case 2:
                printf("Enter Account Number: "); scanf("%d", &acc);
                Customer* c = searchBST(root, acc);
                if (c) printf("Name: %s | Balance: %.2f\n", c->name, c->balance);
                else printf("Customer not found.\n");
                break;
            case 3:
                printf("Enter Account Number to enqueue: "); scanf("%d", &acc);
                enqueue(acc);
                printf("Enqueued.\n");
                break;
            case 4:
                acc = dequeue();
                if (acc != -1) printf("Dequeued Account: %d\n", acc);
                break;
            case 5:
                viewAllCustomers();
                break;
            case 0:
                return;
        }
    }
}

void customerMenu(Customer* cust) {
    int choice;
    float amt;
    char type[10];
    while (1) {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Balance\n2. Deposit\n3. Withdraw\n4. View Transactions\n0. Logout\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Balance: %.2f\n", cust->balance);
                break;
            case 2:
                printf("Enter deposit amount: "); scanf("%f", &amt);
                cust->balance += amt;
                strcpy(type, "Deposit");
                pushTransaction(&cust->transactions, type, amt);
                printf("Deposit successful.\n");
                break;
            case 3:
                printf("Enter withdraw amount: "); scanf("%f", &amt);
                if (amt > cust->balance) {
                    printf("Insufficient balance.\n");
                } else {
                    cust->balance -= amt;
                    strcpy(type, "Withdraw");
                    pushTransaction(&cust->transactions, type, amt);
                    printf("Withdrawal successful.\n");
                }
                break;
            case 4:
                viewTransactions(cust->transactions);
                break;
            case 0:
                return;
        }
    }
}

// ===== Main Program =====
int main() {
    int choice, acc;
    char pass[20];
    while (1) {
        printf("\n--- Bank Management System ---\n");
        printf("1. Bank Officer Login\n2. Customer Login\n0. Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                bankOfficerMenu();
                break;
            case 2:
                printf("Enter Account Number: "); scanf("%d", &acc);
                printf("Enter Password: "); scanf("%s", pass);
                Customer* cust = searchBST(root, acc);
                if (cust && strcmp(cust->password, pass) == 0) {
                    customerMenu(cust);
                } else {
                    printf("Invalid login.\n");
                }
                break;
            case 0:
                exit(0);
        }
    }
}
