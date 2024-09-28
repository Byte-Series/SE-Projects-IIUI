#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Account class definition
class Account {
private:
    int accountNumber;
    string accountHolder;
    double balance;

public:
    // Default Constructor
    Account() : accountNumber(0), accountHolder(""), balance(0.0) {}

    // Constructor
    Account(int accNum, string holder) {
        accountNumber = accNum;
        accountHolder = holder;
        balance = 0.0;
    }

    // Deposit money
    void deposit(double amount);
    
    // Withdraw money
    void withdraw(double amount);
    
    // Get balance
    double getBalance() const;
    
    // Get account holder name
    string getAccountHolder() const;
    
    // Get account number
    int getAccountNumber() const;
    
    // Function to write account details to a file
    void writeToFile(ofstream &file);
    
    // Function to read account details from a file
    static Account readFromFile(ifstream &file);
    
    // Function to search account details
    static Account* searchAccount(int accNum);
};

// Define the functions outside the class using :: operator
void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited: $" << amount << endl;
    } else {
        cout << "Invalid deposit amount!" << endl;
    }
}

void Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        cout << "Withdrawn: $" << amount << endl;
    } else {
        cout << "Insufficient funds or invalid amount!" << endl;
    }
}

double Account::getBalance() const {
    return balance;
}

string Account::getAccountHolder() const {
    return accountHolder;
}

int Account::getAccountNumber() const {
    return accountNumber;
}

void Account::writeToFile(ofstream &file) {
    file << accountNumber << " " << accountHolder << " " << balance << endl;
}

Account Account::readFromFile(ifstream &file) {
    int accNum;
    string holder;
    double bal;

    if (file >> accNum >> holder >> bal) {
        Account account(accNum, holder);
        account.balance = bal; // Set balance after creating account
        return account;
    }
    throw runtime_error("Failed to read account from file");
}

// Static function to search for an account by account number
Account* Account::searchAccount(int accNum) {
    ifstream file("accounts.txt");
    if (!file) {
        cout << "Error opening file!" << endl;
        return nullptr;
    }

    while (!file.eof()) {
        try {
            Account account = readFromFile(file);
            if (account.getAccountNumber() == accNum) {
                file.close();
                return new Account(account); // Return a copy of the found account
            }
        } catch (...) {
            break;
        }
    }

    file.close();
    return nullptr; // Not found
}

// Main function
int main() {
    Account* account = nullptr;
    int choice;
    int accNum;
    string holder;
    Account* foundAccount = nullptr; // Declare foundAccount here

    do {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Save Account to File\n";
        cout << "6. Load Accounts from File\n";
        cout << "7. Search Account\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Account Holder Name: ";
                cin >> holder;
                account = new Account(accNum, holder);
                cout << "Account created for " << holder << endl;
                break;

            case 2:
                if (account != nullptr) {
                    double depositAmount;
                    cout << "Enter amount to deposit: ";
                    cin >> depositAmount;
                    account->deposit(depositAmount);
                } else {
                    cout << "No account found! Please create an account first.\n";
                }
                break;

            case 3:
                if (account != nullptr) {
                    double withdrawAmount;
                    cout << "Enter amount to withdraw: ";
                    cin >> withdrawAmount;
                    account->withdraw(withdrawAmount);
                } else {
                    cout << "No account found! Please create an account first.\n";
                }
                break;

            case 4:
                if (account != nullptr) {
                    cout << "Current Balance: $" << account->getBalance() << endl;
                } else {
                    cout << "No account found! Please create an account first.\n";
                }
                break;

            case 5: {
                ofstream file("accounts.txt", ios::app);
                if (file) {
                    account->writeToFile(file);
                    cout << "Account saved to file.\n";
                } else {
                    cout << "Error opening file for writing!\n";
                }
                file.close();
                break;
            }

            case 6: {
                ifstream file("accounts.txt");
                if (file) {
                    while (!file.eof()) {
                        try {
                            Account loadedAccount = Account::readFromFile(file);
                            cout << "Loaded Account: " << loadedAccount.getAccountHolder() 
                                 << " (Account Number: " << loadedAccount.getAccountNumber() 
                                 << ", Balance: $" << loadedAccount.getBalance() << ")\n";
                        } catch (const runtime_error&) {
                            break; // Break on error
                        }
                    }
                } else {
                    cout << "Error opening file for reading!\n";
                }
                file.close();
                break;
            }

            case 7:
                cout << "Enter Account Number to search: ";
                cin >> accNum;
                foundAccount = Account::searchAccount(accNum); // Assign here
                if (foundAccount) {
                    cout << "Account found: " << foundAccount->getAccountHolder() 
                         << " (Account Number: " << foundAccount->getAccountNumber() 
                         << ", Balance: $" << foundAccount->getBalance() << ")\n";
                    delete foundAccount; // Clean up
                } else {
                    cout << "Account not found!\n";
                }
                break;

            case 8:
                cout << "Exiting the system. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    delete account; // Clean up allocated memory
    return 0;
}
