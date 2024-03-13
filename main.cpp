#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iomanip>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include "patch.h"

int a_n;
std::string account_number, first, last, middle, address, birthday, pin, gender, account_str;
char account;
float balance, initial_deposit;
bool isValid, isEighteen;



void saveAccountToData(const std::string& accountNumber) 
{
    if (mkdir("accounts") != 0 && errno != EEXIST) 
    {
        std::cerr << "Error: Unable to create the 'accounts' directory." << std::endl;
        return;
    }

    std::ofstream file(("accounts/" + accountNumber + ".csv").c_str());

    if (file.is_open()) 
    {
        file << std::fixed << std::setprecision(2);
        file << accountNumber << "," << first << "," << last << "," << middle << "," << address << "," << birthday << "," << pin << "," << gender << "," << account << "," << balance << "," << initial_deposit << std::endl;
        file.close();
    } 
    else 
    {
        std::cerr << "Error: Unable to open the file " << accountNumber << ".csv for writing." << std::endl;
    }
}



void loadAccountData(const std::string& accountNumber)
{
    std::ifstream file(("accounts/" + accountNumber + ".csv").c_str());

    if (file.is_open())
    {
        std::string line,temp_account,temp_balance,temp_initial;
        const char* arr_acc;
        getline(file, line);

        std::istringstream ss(line);

        getline(ss, account_number, ',');
        getline(ss, first, ',');
        getline(ss, last, ',');
        getline(ss, middle, ',');
        getline(ss, address, ',');
        getline(ss, birthday, ',');
        getline(ss, pin, ',');
        getline(ss, gender, ',');
        getline(ss, temp_account, ',');
        getline(ss, temp_balance, ',');
        getline(ss, temp_initial, ',');
        arr_acc = temp_account.c_str();
        account = arr_acc[0];
        std::cout << account;
        if(account == 's' || account == 'S')
            account_str = "Savings";
        else if(account == 'f' || account == 'F')
            account_str = "Current";
        balance = patch::to_float(temp_balance);
        initial_deposit = patch::to_float(temp_initial);
    }
}

void clearScreen() 
{
    system("cls");
}

void displayAccessDenied()
{
    clearScreen();
    std::cout << "****************************************" << std::endl;
    std::cout << "*          Access Denied!               *" << std::endl;
    std::cout << "*      Incorrect PIN. Please try again. *" << std::endl;
    std::cout << "****************************************" << std::endl;
    sleep(2);
}

void displayTransactionSuccess(std::string transactionType, float amount) 
{
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "|       " << transactionType << " Successful      |" << std::endl;
    std::cout << "|   Updated Balance: P" << std::fixed << std::setprecision(2) << balance << "   |" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Amount " << transactionType << ": P" << std::fixed << std::setprecision(2) << amount << std::endl;
    std::cout << "==================================" << std::endl;
    system("pause");
}


void displayWelcomeScreen() 
{
    clearScreen();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|              Welcome to               |" << std::endl;
    std::cout << "|               Snickers                |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void displayMainMenu() 
{
    std::cout << "Main Menu:" << std::endl;
    std::cout << "1. Open a New Account" << std::endl;
    std::cout << "2. Balance Inquiry" << std::endl;
    std::cout << "3. Deposit" << std::endl;
    std::cout << "4. Withdraw" << std::endl;
    std::cout << "5. Account Information" << std::endl;
    std::cout << "6. Delete Account" << std::endl;
    std::cout << "7. Change PIN" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayBalance() 
{
    clearScreen();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|          Balance Inquiry             |" << std::endl;
    std::cout << "|          Current Balance: P" << std::fixed << std::setprecision(2) << balance << "   |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    system("pause");
}

void displayNewAccountMenu() 
{
    clearScreen();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|        Open a New Account            |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void displayPINGenerated() 
{
    clearScreen();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|           PIN Generated              |" << std::endl;
    std::cout << "|           Your PIN is: " << pin << "          |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    system("pause");
}

bool checkPin() 
{
    clearScreen();
    std::string guess;
    
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|               Enter PIN               |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "Enter your PIN: ";
    std::cin >> guess;

    if (strcmp(guess.c_str(), pin.c_str()) == 0)
    {
        std::cout << "PIN Verified. Access Granted." << std::endl;
        sleep(2);
        return true;
    }
    else
    {
        displayAccessDenied();
        return false;
    }
}

void changePin()
{
    int new_pin;
    if(checkPin())
    {
        while(true)
        {
            std::cout << "New pin: ";
            std::cin >> new_pin;
            if(std::cin.fail() || new_pin >= 1000 || new_pin < 0)
            {
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n'); 
                std::cout << "Invalid Input. Enter a valid PIN. " << std::endl;
            }
            else
                break;
        }
        pin = patch::to_string(new_pin);
        pin = pin.size() < 4 ? std::string(4 - pin.size(), '0') + pin : pin;
        saveAccountToData(account_number);
        displayPINGenerated();
    }
    else
    {
        displayAccessDenied();
        system("pause");
        return;
    }
}

void deleteAccount()
{
    clearScreen();
    char choice;
    if(checkPin())
    {
        while(true)
        {
            clearScreen();
            std::cout << "\n========================================" << std::endl;
            std::cout << "|         Delete Your Account          |" << std::endl;
            std::cout << "========================================" << std::endl;
            std::cout << "Are you sure you want to delete your account? (Enter 'y' if yes and 'n' if no)\nInput: ";
            std::cin >> choice;
            if(choice == 'y' || choice == 'Y')
            {
                std::string filename = "accounts/" + account_number + ".csv";
                if (remove(filename.c_str()) != 0)
                {
                    std::cerr << "Error: Unable to delete the file " << filename << std::endl;
                }
                account_number.clear();
                first.clear();
                last.clear();
                middle.clear();
                address.clear();
                birthday.clear();
                pin.clear();
                gender.clear();
                account_str.clear();
                account = '\0';
                balance = 0;
                initial_deposit = 0;
                isValid = false;
                isEighteen = false;
                std::cout << "\n========================================" << std::endl;
                std::cout << "|       Account Deleted Successfully    |" << std::endl;
                std::cout << "========================================\n" << std::endl;
                system("pause");
                break;
            }
            else if(choice == 'n' || choice == 'N')
            {
                std::cout << "\n========================================" << std::endl;
                std::cout << "|     Account Deletion Canceled        |" << std::endl;
                std::cout << "========================================\n" << std::endl;
                system("pause");
                break;
            }
            else
            {

                std::cout << "\n========================================" << std::endl;
                std::cout << "|        Invalid Input! Try Again       |" << std::endl;
                std::cout << "========================================\n" << std::endl;
                system("pause");
            }
        } 
    }
    else
    {
        displayAccessDenied();
        return;
    }
}

void withdraw()
{
    if(checkPin())
    {
        bool dec = true;
        float withdraw_money;
        while(dec)
        {
            clearScreen();
            std::cout << "==================================" << std::endl;
            std::cout << "            WITHDRAW               " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "Withdraw: P";
            std::cin >> withdraw_money;
            if(withdraw_money <= balance && withdraw_money >= 300 && account == 's' || account == 'S')
            {
                balance -= withdraw_money;
                dec = false;
                saveAccountToData(account_number);
                displayTransactionSuccess("Withdraw", withdraw_money);
            }
            else if(withdraw_money >= 500 && withdraw_money <= balance && account == 'c' || account == 'C')
            {
                balance -= withdraw_money;
                dec = false;
                saveAccountToData(account_number);
                displayTransactionSuccess("Withdraw", withdraw_money);
            }
            else if (withdraw_money == -1)
            {
                std::cout << "Transaction canceled." << std::endl;
                dec = false;
                system("pause");
            }
            else
            {
                std::cout << "Invalid input. ";
                if(withdraw_money > balance)
                    std::cout << "Insufficient Balance. ";
                std::cout << "Enter -1 to cancel the transaction. " << std::endl;
            }
        }
    }
    else
    {
        displayAccessDenied();
        system("pause");
    }  
}

void deposit()
{

    if(checkPin())
    {
        bool dec = true;
        float deposit_money;
        while(dec)
        {
            clearScreen();
            std::cout << "==================================" << std::endl;
            std::cout << "            DEPOSIT               " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "Deposit: P";
            std::cin >> deposit_money;
            if(deposit_money >= 300 && account == 's' || account == 'S')
            {
                balance += deposit_money;
                dec = false;
                saveAccountToData(account_number);
                displayTransactionSuccess("Deposit", deposit_money);
            }
            else if(deposit_money >= 500 && account == 'c' || account == 'C')
            {
                balance += deposit_money;
                dec = false;
                saveAccountToData(account_number);
                displayTransactionSuccess("Deposit", deposit_money);
            }
            else if (deposit_money == -1)
            {
                std::cout << "Transaction canceled." << std::endl;
                dec = false;
                system("pause");
            }
            else
            {
                std::cout << "\nInvalid Input. Minimum deposit amount for ";
                if(account == 's' || account == 'S')
                    std::cout << "savings account is P300. ";
                else if(account == 'c' || account == 'C')
                    std::cout << "current account is P500. ";
                
                std::cout << "Enter -1 to cancel the transaction. " << std::endl;

            }
                
        }
    }
    else
    {
        displayAccessDenied();
        system("pause");
    }   
}

void balance_check() 
{
    clearScreen();
    if (checkPin()) 
    {
        displayBalance();
    }
    else 
    {
        displayAccessDenied();
        system("pause");
    }
}

void viewAccountInformation()
{
    clearScreen();
    if (checkPin()) 
    {
        clearScreen();
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "|        Account Information           |" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "| Account Number: " << account_number << std::endl;
        std::cout << "| Name: " << first << " " << middle << " " << last << std::endl;
        std::cout << "| Address: " << address << std::endl;
        std::cout << "| Birthdate: " << birthday << std::endl;
        std::cout << "| Gender: " << gender << std::endl;
        std::cout << "| Account Type: " << account_str << std::endl;
        std::cout << "| Initial Deposit: P" << std::fixed << std::setprecision(2) << initial_deposit << std::endl;
        std::cout << "| Current Balance: P" << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        system("pause");
    }
    else 
    {
        displayAccessDenied();
        system("pause");
    }
}

void pin_generator() 
{
    a_n = rand() % 10000;
    pin = patch::to_string(a_n);
    pin = pin.size() < 4 ? std::string(4 - pin.size(), '0') + pin : pin;

    sleep(3);
    displayPINGenerated();
}

void account_type() 
{
    bool dec = true;
    char choice;
    double deposit_money;
    while (dec) 
    {
        std::cout << "Choose an account type:" << std::endl;
        std::cout << "S - Savings Account (Minimum deposit: P5000)" << std::endl;
        std::cout << "C - Current Account (Minimum deposit: P10000)" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 'S' || choice == 's') 
        {
            do
            {
                std::cout << "Enter the initial deposit amount (P5000 or more): P";
                std::cin >> deposit_money;

                if(deposit_money < 5000)
                    std::cout << "Initial deposit is not sufficient. Please try again." << std::endl;                
            } while(deposit_money < 5000);

            balance = deposit_money;
            initial_deposit = deposit_money;
            account = choice;
            account_str = "Savings";
            dec = false;
        } 
        else if (choice == 'C' || choice == 'c') 
        {
            do
            {
                std::cout << "Enter the initial deposit amount (P10000 or more): P";
                std::cin >> deposit_money;

                if(deposit_money < 10000)
                    std::cout << "Initial deposit is not sufficient. Please try again." << std::endl;                
            } while(deposit_money < 10000);

            balance = deposit_money;
            initial_deposit = deposit_money;
            account = choice;
            account_str = "Current";
            dec = false;
        } 
        else 
        {
            std::cout << "Invalid input. Please choose 'S' for Savings or 'C' for Current Account." << std::endl;
        }
    }
    std::cout << "Account type selected successfully!" << std::endl;
    system("pause");
}

void gender_checker() 
{
    bool isAllowed = true;
    char input;
    while (isAllowed) 
    {
        std::cout << "Gender (M/F): ";
        std::cin >> input;
        if (input == 'M' || input == 'm') 
        {
            gender = "Male";
            isAllowed = false;
        } 
        else if (input == 'F' || input == 'f') 
        {
            gender = "Female";
            isAllowed = false;
        } 
        else 
        {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "Enter a valid gender." << std::endl;
        }
    }
}

void birthdate() 
{
    bool dec = true;
    while (dec) 
    {
        std::cout << "Birthdate (MM/DD/YYYY): ";
        std::cin >> birthday;
        if (birthday.size() == 10) 
        {
            std::istringstream bd(birthday);
            int month, day, year;
            char slash;

            if (bd >> month >> slash >> day >> slash >> year && slash == '/') 
            {
                if (month >= 1 && month <= 12 && day >= 1 && day <= 31 && year >= 1900 && (2024 - year) >= 18) 
                {
                    isValid = true;
                    isEighteen = true;
                    dec = false;
                } 
                else if (2024 - year < 18) 
                {
                    clearScreen();
                    std::cout << "\n========================================" << std::endl;
                    std::cout << "|    Sorry, You Are Not Eligible.       |" << std::endl;
                    std::cout << "|   Your Age is Not Allowed to Open     |" << std::endl;
                    std::cout << "|           an Account.                 |" << std::endl;
                    std::cout << "========================================\n" << std::endl;
                    system("pause");
                    return;
                }
            } 
            else 
            {
                std::cout << "Invalid Input. Enter a valid birthdate." << std::endl;
            }
        } 
        else 
        {
            std::cout << "Invalid Input. Enter a valid birthdate." << std::endl;
        }
    }
}

void personal_info() 
{
    clearScreen();
    std::cin.ignore();
    
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "|          Personal Information         |" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "First name: ";
    getline(std::cin, first);

    std::cout << "Last name: ";
    getline(std::cin, last);

    std::cout << "Middle name: ";
    getline(std::cin, middle);

    std::cout << "Address: ";
    getline(std::cin, address);

    birthdate();
    if(isEighteen)
        gender_checker();
}

bool accountExists(const std::string& accountNumber) 
{
    std::ifstream file(("accounts/" + accountNumber + ".csv").c_str());

    if (file.is_open()) 
    {
        std::string line;
        getline(file, line);
        
        std::istringstream ss(line);
        std::string actual_acc_num, actual_pin;
        int index = 0;

        while(getline(ss, actual_acc_num, ','))
        {
            if(index == 0)
            {
                if (actual_acc_num == accountNumber)
                {
                    for (int i = 0; i <= 5; ++i)
                    { 
                    getline(ss, actual_pin, ',');
                    }
                    pin = actual_pin;
                    file.close();
                    return true;
                }
            }
            index = (index + 1) % 13;
        }
    file.close();
	}
	return false;
}

void accnum_generator() 
{
    clearScreen();
    std::string temp_acc_num;
    bool exists = true;

    while (exists) 
    {
        a_n = rand() % 10000;
        temp_acc_num = patch::to_string(a_n);
        temp_acc_num = temp_acc_num.size() < 4 ? std::string(4 - temp_acc_num.size(), '0') + temp_acc_num : temp_acc_num;

        if (!accountExists(temp_acc_num)) 
        {
            account_number = temp_acc_num;
            exists = false;
        }
    }
}

void new_account() 
{
    clearScreen();
    accnum_generator();
    personal_info();
    if (isValid) 
    {
        account_type();
        pin_generator();
        saveAccountToData(account_number);
    } 
    else 
    {
        return;
    }
    clearScreen();
}

void check_account()
{
    char decision;
    std::string temp_acc_num;
    displayWelcomeScreen();;

    std::cout << "Do you want to log in with your existing account? (Y/N): ";
    std::cin >> decision;


    if(toupper(decision) == 'Y')
    {
        std::cout << "Enter your account number: ";
        std::cin >> temp_acc_num;

        if(accountExists(temp_acc_num))
        {
            loadAccountData(temp_acc_num);
            isValid = true;
            sleep(2);
        }
        else
        {
            std::cout << "There is no account in our data.";
            sleep(2);
        }

    }

}

int main(void) 
{
    int choice;
    bool dec = true;
    srand(time(0));
    check_account();

    while (dec) 
    {
        displayWelcomeScreen();
        displayMainMenu();
        std::cin >> choice;

        switch (choice) 
        {
            case 1:
                new_account();
                break;
            case 2:
                if(isValid)
                    balance_check();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 3:
                if(isValid)
                    deposit();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 4:
                if (isValid)
                    withdraw();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 5:
                if(isValid)
                    viewAccountInformation();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 6:
                if(isValid)
                    deleteAccount();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 7:
                if(isValid)
                    changePin();
                else
                {
                    std::cout << "Invalid input. Please open a new account first." << std::endl;
                    system("pause");  
                }
                break;
            case 8:
                dec = false;
                clearScreen();
                std::cout << "\n========================================" << std::endl;
                std::cout << "|        Thank you for using ATM.       |" << std::endl;
                std::cout << "|              Goodbye!                 |" << std::endl;
                std::cout << "========================================\n" << std::endl;
                sleep(2);
                break;
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
                system("pause");
        }
        clearScreen();
    }

    return 0;
}
