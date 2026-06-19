# ATM Simulator in C

A console-based ATM Simulator written in C that supports account creation, PIN authentication, deposit, withdraw, transfer, balance inquiry, transaction history, and receipt generation using file handling and basic security features.

## Project Overview

This project simulates a simple ATM system for educational purposes. It uses structures, file handling, transaction logic, and PIN-based security to manage account operations in a console application.

## Features

- Create new ATM account with 4-digit PIN.
- Authenticate user with PIN verification.
- Allow deposit, withdrawal, and money transfer.
- Check account balance.
- View last 10 transactions.
- Change PIN securely.
- Generate transaction receipt.
- Save account data in binary format.
- Save transaction history in text format.
- Use basic PIN encryption and decryption.

## Concepts Used

- File handling with `fopen`, `fclose`, `fread`, and `fwrite`.
- Structures for accounts and transactions.
- Conditional statements and loops for transaction logic.
- PIN authentication with limited attempts.
- Basic encryption and decryption for PIN storage.
- Console-based menu navigation.

## Account Structure

```c
typedef struct {
    int accountNumber;
    char accountHolder;
    char pin;[3]
    float balance;
    char accountType;[4]
    char status;[5]
    long lastTransaction;
    int transactionCount;
} Account;
```

## Transaction Structure

```c
typedef struct {
    int accountNumber;
    char type;[4]
    float amount;
    long timestamp;
    char description;
    char balanceAfter;
} Transaction;
```

## Security Features

### PIN Authentication
- PIN must be exactly 4 digits.
- Maximum 3 attempts allowed.
- After 3 wrong attempts, the account gets locked.
- Wrong PIN shows `Access Denied`.

### PIN Encryption
The project uses basic PIN encryption and decryption for simulation purposes only.

## Transaction Limits

- Maximum deposit: ₹100,000
- Maximum withdrawal: ₹50,000
- Maximum transfer: ₹100,000
- Minimum balance: ₹500

## File Handling

### Accounts File
- File name: `atm_accounts.dat`
- Type: Binary file
- Used to store account details

### Transactions File
- File name: `atm_transactions.txt`
- Type: Text file
- Used to store transaction records

## Menu Options

After authentication, the ATM menu will display:

1. Deposit Cash  
2. Withdraw Cash  
3. Transfer Money  
4. Check Balance  
5. Transaction History  
6. Change PIN  
7. Print Receipt  
8. Exit  

## How to Compile

Use GCC to compile the program:

```bash
gcc atm.c -o atm.exe
```

If you are using Linux or Mac:

```bash
gcc atm.c -o atm
```

## How to Run

### Windows PowerShell
```powershell
.\atm.exe
```

### Linux / Mac
```bash
./atm
```

## Sample Usage

### Create Account
- Account Number: 1234567890
- Name: Rahul Kumar
- PIN: 1234
- Account Type: Savings
- Balance: ₹5000

### Deposit
- Amount: ₹10000
- New Balance: ₹15000

### Withdraw
- Amount: ₹5000
- New Balance: ₹10000

### Transfer
- To Account: 9876543210
- Amount: ₹3000
- New Balance: ₹7000

## Sample Output Messages

- `Account created successfully`
- `Authentication successful`
- `Deposited: ₹10,000`
- `Withdrawn: ₹5,000`
- `Transferred: ₹3,000`
- `Access Denied! Wrong PIN`
- `Account locked!`

## Transaction History

The program keeps a record of recent transactions such as:

1. Deposit ₹10,000 - Balance: ₹15,000
2. Withdraw ₹5,000 - Balance: ₹10,000
3. Transfer ₹3,000 - Balance: ₹7,000

## Receipt Format

```text
================================
       ATM RECEIPT
================================
Account: 1234567890
Transaction: Withdraw
Amount: ₹5,000.00
Balance: ₹10,000.00
Date: 18/06/2026
================================
        Thank you!
================================
```

## File Structure

```bash
ATMSimulator/
│
├── atm.c
├── atm_accounts.dat
├── atm_transactions.txt
├── README.md
└── Makefile
```

## Validation Rules

- PIN must contain exactly 4 digits.
- Deposit amount must be greater than 0.
- Withdraw amount must be within daily limit and available balance.
- Transfer amount must not exceed available balance or limit.
- Balance should not go below ₹500.

## Notes

- This is only a simulation for learning purposes.
- No real banking or internet connectivity is used.
- Encryption is basic and intended only for practice.
- All operations are console-based.

## Requirements

- C compiler (GCC recommended)
- Standard C libraries only:
  - `stdio.h`
  - `stdlib.h`
  - `string.h`
  - `time.h`

## Makefile

```make
all:
	gcc atm.c -o atm.exe

run:
	.\atm.exe

clean:
	del atm.exe atm_accounts.dat atm_transactions.txt
```

## Author

Created for educational use to practice C programming, file handling, transaction logic, and basic security concepts.

## License

This project is free to use for learning and academic purposes.
