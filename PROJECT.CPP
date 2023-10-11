#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Customer {
public:
    int customerID;
    string customerName;
    string phoneNumber;
};

class Purchase {
public:
    int billNo;
    int customerID;
    double totalAmount;
    vector<string> purchasedItems;
};

class head {
private:
    char Iname[50][50];

public:
    int totalitems;
    double Qty[3];
    double price[3];
    int vatprice[3];
    int tprice[3];

    const char* getItemName(int index) const {
        return Iname[index];
    }

    void input();
    void output(int billNumber, int customerID);
};

class vat : public head {
    float vats;

public:
    void vatcal();
    void outputs(vector<Customer>& customers, vector<Purchase>& purchases, int billNumber, int customerID);
    void sum();
};

int generateBillNumber() {
    static int nextBillNumber = 1;
    return nextBillNumber++;
}

void head::input() {
    system("CLS");
    cout << "\nEnter the number of items: ";
    cin >> totalitems;

    for (int i = 0; i < totalitems; i++) {
        cout << "\nEnter the name of item " << i + 1 << ": ";
        cin.ignore();
        cin.getline(Iname[i], sizeof(Iname[i]));
        cout << "Enter quantity: ";
        cin >> Qty[i];
        cout << "Enter price of item " << i + 1 << ": ";
        cin >> price[i];
        tprice[i] = Qty[i] * price[i];
    }
}

void head::output(int billNumber, int customerID) {
    int a;

    ifstream infile("COUNT.TXT");
    infile >> a;

    ofstream outfile("COUNT.TXT");
    a += 1;
    outfile << a;
    outfile.close();

    ofstream outfile2("HIS.TXT", ios::app);
    outfile2 << endl << "Bill No.: " << a << endl;
    outfile2 << "Customer ID: " << customerID << endl;
    cout << "\n";
    cout << "Name of Item\tQuantity   Price  Total Price\n";
    for (int i = 0; i < totalitems; i++) {
        outfile2 << "Name: " << Iname[i] << " Qty: " << Qty[i] << " Price: " << tprice[i] << endl;
        cout << Iname[i] << "\t\t" << Qty[i] << "\t   " << price[i] << "\t   " << tprice[i] << '\n';
    }
}
void vat::vatcal() {
    input();
    for (int i = 0; i < totalitems; i++) {
        if (price[i] <= 100.00) {
            vatprice[i] = tprice[i] + (0.03 * tprice[i]);
        } else {
            vatprice[i] = tprice[i] + (0.1 * tprice[i]);
        }
    }
}
void vat::outputs(vector<Customer>& customers, vector<Purchase>& purchases, int billNumber, int customerID) {
    output(billNumber, customerID);

    float cash = 0, sum = 0, qty = 0, sumt = 0;

    for (int i = 0; i < totalitems; i++) {
        sumt += tprice[i];
        sum += vatprice[i];
        qty += Qty[i];
    }
    cout << "\nTotal:";
    cout << "\n------------------------------------------------------------------------------";
    cout << "\n\tQuantity= " << qty << "\t\t Sum= " << sumt << "\tWith Vat:" << sum;
    cout << "\n------------------------------------------------------------------------------";

pay:
    cout << "\n\n\t\t\t****PAYMENT SUMMARY****\n";
    cout << "\n\t\t\tTotal cash given: ";
    cin >> cash;

    if (cash >= sum)
        cout << "\n\t\t\tTotal cash repaid: " << cash - sum << '\n';
    else {
        cout << "\n\t\t\tCash given is less than the total amount!!!";
        goto pay;
    }
}
bool authenticateUser(const string& username, const string& password) {
    return (username == "12345" && password == "12345");
}

int main() {
     cout << "\n\t\t\t\tWelcome to Billing Area";
    string username, password;
    cout << "\n\n\t\t\tUsername: ";
    
    cin >> username;
   
    cout << "\n\n\t\t\tPassword: ";
    
    cin >> password;
 

    if (authenticateUser(username, password)) {
        vat obj;
        char opt;
        vector<Customer> customers;
        vector<Purchase> purchases;
        int customerID = 0;

        ifstream fin;
        char ch;

        do {
            system("PAUSE");
            //system("clear");
            cout << "\n\n\t\t\t+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+";
            cout << "\n\t\t\t\tShop Billing Area";
            cout << "\n\t\t\t+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+";
            cout << "\n\n\t\t\tWhat you want to do?";
            cout << "\n\t\t\t1.\tTo enter a new entry";;
            cout << "\n\t\t\t2.\tTo view previous entries";
            cout << "\n\t\t\t1.\tExit\n";
            cout << "\n\nEnter your option: ";
            cin >> opt;

            switch (opt) {
            case '1':
                obj.vatcal();
                obj.outputs(customers, purchases, generateBillNumber(), customerID);
                break;
            case '2':
                fin.open("HIS.TXT", ios::in);
                while (fin.get(ch)) {
                    cout << ch;
                }
                fin.close();
                break;
            case '3':
                exit(0);
            default:
                cout << "\a";
            }
        } while (opt != '3');
    } else {
        cout << "Invalid username or password. Access denied." << endl;
    }

    return 0;
}
