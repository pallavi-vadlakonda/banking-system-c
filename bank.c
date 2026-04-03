#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "accounts.dat"
#define MAX_ACCOUNTS 100

typedef struct {
    int acc_no;
    char name[50];
    double balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int num_accounts = 0;

void loadAccounts() {
    FILE *file = fopen(FILENAME, "rb");
    if (file) {
        fread(&num_accounts, sizeof(int), 1, file);
        if (num_accounts > 0 && num_accounts <= MAX_ACCOUNTS) {
            fread(accounts, sizeof(Account), num_accounts, file);
        }
        fclose(file);
    }
}

void saveAccounts() {
    FILE *file = fopen(FILENAME, "wb");
    if (file) {
        fwrite(&num_accounts, sizeof(int), 1, file);
        if (num_accounts > 0) {
            fwrite(accounts, sizeof(Account), num_accounts, file);
        }
        fclose(file);
    }
}

int findAccount(int acc_no) {
    for (int i = 0; i < num_accounts; i++) {
        if (accounts[i].acc_no == acc_no) return i;
    }
    return -1;
}

void createAccount() {
    if (num_accounts >= MAX_ACCOUNTS) {
        printf("Maximum accounts reached!\n");
        return;
    }
    Account acc;
    printf("Enter account number: ");
    scanf("%d", &acc.acc_no);
    if (findAccount(acc.acc_no) != -1) {
        printf("Account number already exists!\n");
        return;
    }
    printf("Enter name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter initial deposit: ");
    scanf("%lf", &acc.balance);
    if (acc.balance < 0) {
        printf("Initial deposit cannot be negative!\n");
        return;
    }
    accounts[num_accounts++] = acc;
    saveAccounts();
    printf("Account created successfully!\n");
}

void deposit() {
    int acc_no;
    double amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter deposit amount: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Deposit amount must be positive!\n");
        return;
    }
    accounts[idx].balance += amount;
    saveAccounts();
    printf("Deposited %.2lf successfully. New balance: %.2lf\n", amount, accounts[idx].balance);
}

void withdraw() {
    int acc_no;
    double amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Withdrawal amount must be positive!\n");
        return;
    }
    if (amount > accounts[idx].balance) {
        printf("Insufficient balance! Current balance: %.2lf\n", accounts[idx].balance);
        return;
    }
    accounts[idx].balance -= amount;
    saveAccounts();
    printf("Withdrew %.2lf successfully. New balance: %.2lf\n", amount, accounts[idx].balance);
}

void balanceEnquiry() {
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = findAccount(acc_no);
    if (idx == -1) {
        printf("Account not found!\n");
        return;
    }
    printf("Account: %d, Name: %s, Balance: %.2lf\n", accounts[idx].acc_no, accounts[idx].name, accounts[idx].balance);
}

void displayAll() {
    if (num_accounts == 0) {
        printf("No accounts found!\n");
        return;
    }
    printf("\nAcc No\tName\t\t\tBalance\n");
    printf("--------------------------------\n");
    for (int i = 0; i < num_accounts; i++) {
        printf("%d\t%s\t\t%.2lf\n", accounts[i].acc_no, accounts[i].name, accounts[i].balance);
    }
}

int main() {
    loadAccounts();
    int choice;
    printf("=== Bank Account Management System ===\n");
    do {
        printf("\n1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Balance Enquiry\n");
        printf("5. Display All Accounts\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: balanceEnquiry(); break;
            case 5: displayAll(); break;
            case 0: printf("Thank you for using the system!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
    saveAccounts();
    return 0;
}
