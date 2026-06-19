#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ACCOUNTS_FILE "atm_accounts.dat"
#define TRANSACTIONS_FILE "atm_transactions.txt"
#define MAX_ACCOUNTS 100
#define MAX_WITHDRAW 50000
#define MAX_DEPOSIT 100000
#define MIN_BALANCE 500

typedef struct
{
    int accountNumber;
    char accountHolder[50];
    char pin[10];
    float balance;
    char accountType[20];
    char status[10];
    long lastTransaction;
    int transactionCount;
} Account;

typedef struct
{
    int accountNumber;
    char type[20];
    float amount;
    long timestamp;
    char description[100];
    float balanceAfter;
} Transaction;

/* GLOBAL DATA */
Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

/* FUNCTIONS */
void encryptPIN(char pin[]);
void decryptPIN(char pin[]);
int findAccount(int accNum);

int createAccount();
int authenticate(int accNum, char pin[]);
float deposit(float balance, float amount);
float withdraw(float balance, float amount);
float transferMoney(int fromAcc, int toAcc, float amount);

void printReceipt(char type[], float amount, float balance, int acc);
void saveAccountsToFile();
void loadAccountsFromFile();
void saveTransaction(int acc, char type[], float amount, float bal);

void ATMMenu(int accIndex);

/* PIN ENCRYPTION */
void encryptPIN(char pin[])
{
    for (int i = 0; i < 4; i++)
        pin[i] = pin[i] + 10;
}

void decryptPIN(char pin[])
{
    for (int i = 0; i < 4; i++)
        pin[i] = pin[i] - 10;
}

/* FIND ACCOUNT */
int findAccount(int accNum)
{
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accNum)
            return i;
    }
    return -1;
}

/* CREATE ACCOUNT */
int createAccount()
{
    if (accountCount >= MAX_ACCOUNTS)
    {
        printf("Bank Full!\n");
        return -1;
    }

    Account a;

    printf("Enter Account Number: ");
    scanf("%d", &a.accountNumber);

    printf("Enter Name: ");
    scanf(" %[^\n]", a.accountHolder);

    printf("Enter 4-digit PIN: ");
    scanf("%s", a.pin);

    printf("Enter Account Type: ");
    scanf("%s", a.accountType);

    a.balance = 0;
    strcpy(a.status, "ACTIVE");
    a.transactionCount = 0;
    a.lastTransaction = time(NULL);

    encryptPIN(a.pin);

    accounts[accountCount++] = a;

    saveAccountsToFile();

    printf("Account Created Successfully!\n");
    return 0;
}

/* AUTHENTICATION */
int authenticate(int accNum, char pin[])
{
    int idx = findAccount(accNum);

    if (idx == -1)
        return -1;

    char tempPin[10];
    strcpy(tempPin, accounts[idx].pin);
    decryptPIN(tempPin);

    int attempts = 3;

    while (attempts--)
    {
        if (strcmp(pin, tempPin) == 0)
        {
            printf("Login Successful!\n");
            return idx;
        }
        else
        {
            printf("Wrong PIN! Attempts left: %d\n", attempts);
            if (attempts == 0)
            {
                printf("Account Locked!\n");
                return -1;
            }
            printf("Enter PIN again: ");
            scanf("%s", pin);
        }
    }

    return -1;
}

/* DEPOSIT */
float deposit(float balance, float amount)
{
    if (amount <= 0)
        return balance;
    if (amount > MAX_DEPOSIT)
    {
        printf("Max deposit 100000\n");
        return balance;
    }

    balance += amount;
    printf("Deposited: %.2f\n", amount);
    return balance;
}

/* WITHDRAW */
float withdraw(float balance, float amount)
{
    if (amount <= 0)
        return balance;

    if (amount > MAX_WITHDRAW)
    {
        printf("Max withdrawal 50000\n");
        return balance;
    }

    if (amount > balance)
    {
        printf("Insufficient balance!\n");
        return balance;
    }

    if (balance - amount < MIN_BALANCE)
    {
        printf("Must maintain minimum balance 500\n");
        return balance;
    }

    balance -= amount;
    printf("Withdrawn: %.2f\n", amount);
    return balance;
}

/* TRANSFER */
float transferMoney(int fromAcc, int toAcc, float amount)
{
    int toIdx = findAccount(toAcc);
    if (toIdx == -1)
    {
        printf("Target account not found!\n");
        return -1;
    }

    int fromIdx = findAccount(fromAcc);

    if (amount > accounts[fromIdx].balance)
    {
        printf("Insufficient balance!\n");
        return accounts[fromIdx].balance;
    }

    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance += amount;

    printf("Transferred %.2f to %d\n", amount, toAcc);

    return accounts[fromIdx].balance;
}

/* SAVE ACCOUNTS */
void saveAccountsToFile()
{
    FILE *fp = fopen(ACCOUNTS_FILE, "wb");
    fwrite(&accountCount, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(Account), accountCount, fp);
    fclose(fp);
}

/* LOAD ACCOUNTS */
void loadAccountsFromFile()
{
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp == NULL)
        return;

    fread(&accountCount, sizeof(int), 1, fp);
    fread(accounts, sizeof(Account), accountCount, fp);
    fclose(fp);
}

/* TRANSACTION LOG */
void saveTransaction(int acc, char type[], float amount, float bal)
{
    FILE *fp = fopen(TRANSACTIONS_FILE, "a");

    fprintf(fp, "%d | %s | %.2f | %.2f | %ld\n",
            acc, type, amount, bal, time(NULL));

    fclose(fp);
}

/* RECEIPT */
void printReceipt(char type[], float amount, float balance, int acc)
{
    printf("\n========================\n");
    printf("      ATM RECEIPT\n");
    printf("========================\n");
    printf("Account: %d\n", acc);
    printf("Type: %s\n", type);
    printf("Amount: %.2f\n", amount);
    printf("Balance: %.2f\n", balance);
    printf("Time: %ld\n", time(NULL));
    printf("========================\n");
}

/* ATM MENU */
void ATMMenu(int idx)
{
    int choice;
    float amt;

    do
    {
        printf("\n=== ATM MENU ===\n");
        printf("Account: %d\n", accounts[idx].accountNumber);
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Transfer\n");
        printf("4. Check Balance\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            printf("Enter amount: ");
            scanf("%f", &amt);
            accounts[idx].balance = deposit(accounts[idx].balance, amt);
            saveTransaction(accounts[idx].accountNumber, "Deposit", amt, accounts[idx].balance);
            printReceipt("Deposit", amt, accounts[idx].balance, accounts[idx].accountNumber);
            saveAccountsToFile();
            break;

        case 2:
            printf("Enter amount: ");
            scanf("%f", &amt);
            accounts[idx].balance = withdraw(accounts[idx].balance, amt);
            saveTransaction(accounts[idx].accountNumber, "Withdraw", amt, accounts[idx].balance);
            printReceipt("Withdraw", amt, accounts[idx].balance, accounts[idx].accountNumber);
            saveAccountsToFile();
            break;

        case 3:
        {
            int toAcc;
            printf("Enter target account: ");
            scanf("%d", &toAcc);
            printf("Enter amount: ");
            scanf("%f", &amt);

            accounts[idx].balance = transferMoney(accounts[idx].accountNumber, toAcc, amt);
            saveTransaction(accounts[idx].accountNumber, "Transfer", amt, accounts[idx].balance);
            saveAccountsToFile();
            break;
        }

        case 4:
            printf("Balance: %.2f\n", accounts[idx].balance);
            break;
        }

    } while (choice != 5);
}

/* MAIN */
int main()
{
    int choice, accNum;
    char pin[10];

    loadAccountsFromFile();

    printf("=== ATM SYSTEM ===\n");
    printf("1. Create Account\n");
    printf("2. Login\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        createAccount();
        return 0;
    }

    printf("Enter Account Number: ");
    scanf("%d", &accNum);

    printf("Enter PIN: ");
    scanf("%s", pin);

    int idx = authenticate(accNum, pin);

    if (idx != -1)
    {
        ATMMenu(idx);
    }

    saveAccountsToFile();
    return 0;
}