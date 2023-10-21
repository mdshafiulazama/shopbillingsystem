#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class Customer
{
public:
    int customerID;
    string customerName;
    string phoneNumber;

    void input()
    {
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customerName);
        cout << "Enter phone number: ";
        cin >> phoneNumber;
    }
};

class Purchase
{
public:
    int billNo;
    int customerID;
    float totalAmount;
    vector<string> purchasedItems;

    void input(int billNumber, int custID, float amount, const vector<string> &items)
    {
        billNo = billNumber;
        customerID = custID;
        totalAmount = amount;
        purchasedItems = items;
    }
};

class head
{
private:
    char Iname[50][50];

public:
    int totalitems;
    float Qty[3];
    float price[3];
    int vatprice[3];
    int tprice[3];

    const char *getItemName(int index) const
    {
        return Iname[index];
    }

    void input();
    void output(int billNumber, int customerID);
};

class vat : public head
{
    float vats;

public:
    void vatcal();
    void outputs(vector<Customer> &customers, vector<Purchase> &purchases, int billNumber, int customerID);
    void sum();
};

int generateCustomerID(vector<Customer> &customers)
{
    if (customers.empty())
    {
        return 1;
    }
    else
    {
        int highestID = customers.back().customerID;
        return highestID + 1;
    }
}

int generateBillNumber()
{
    static int nextBillNumber = 1;
    return nextBillNumber++;
}

Customer createCustomer(vector<Customer> &customers)
{
    Customer newCustomer;
    newCustomer.customerID = generateCustomerID(customers);
    newCustomer.input();
    return newCustomer;
}

void loadCustomerDetails(vector<Customer> &customers)
{
    ifstream customerFile("CUSTOMERS.TXT");
    if (customerFile)
    {
        Customer customer;
        while (customerFile >> customer.customerID >> ws && getline(customerFile, customer.customerName) && getline(customerFile, customer.phoneNumber))
        {
            customers.push_back(customer);
        }
        customerFile.close();
    }
}

void saveCustomerDetails(const vector<Customer> &customers)
{
    ofstream customerFile("CUSTOMERS.TXT");
    for (const Customer &customer : customers)
    {
        customerFile << customer.customerID << endl;
        customerFile << customer.customerName << endl;
        customerFile << customer.phoneNumber << endl;
    }
    customerFile.close();
}

void head::input()
{
    system("CLS");
    cout << "\nEnter number of items= ";
    cin >> totalitems;

    for (int i = 0; i < totalitems; i++)
    {
        cout << "\nEnter name of item " << i + 1 << ": ";
        cin.ignore();
        cin.getline(Iname[i], sizeof(Iname[i]));
        cout << "Enter quantity: ";
        cin >> Qty[i];
        cout << "Enter price of item " << i + 1 << ": ";
        cin >> price[i];
        tprice[i] = Qty[i] * price[i];
    }
}

void head::output(int billNumber, int customerID)
{
    int a;

    ifstream infile("COUNT.TXT");
    infile >> a;

    ofstream outfile("COUNT.TXT");
    a += 1;
    outfile << a;
    outfile.close();

    ofstream outfile2("HIS.TXT", ios::app);
    outfile2 << endl
             << "Bill No.: " << a << endl;
    outfile2 << "Customer ID: " << customerID << endl;
    outfile2 << "------------------------------------------------------------------------" << endl;
    cout << "\n";
    cout << "Name of Item\tQuantity   Price  Total Price\n";
    for (int i = 0; i < totalitems; i++)
    {
        outfile2 << "Name: " << Iname[i] << " Qty: " << Qty[i] << " Price: " << tprice[i] << endl;
        cout << Iname[i] << "\t\t" << Qty[i] << "\t   " << price[i] << "\t   " << tprice[i] << '\n';
    }

    outfile2 << "------------------------------------------------------------------------" << endl;
    outfile2.close();
}

void vat::vatcal()
{
    input();
    for (int i = 0; i < totalitems; i++)
    {
        if (price[i] <= 100.00)
        {
            vatprice[i] = tprice[i] + (0.03 * tprice[i]);
        }
        else
        {
            vatprice[i] = tprice[i] + (0.1 * tprice[i]);
        }
    }
}

void vat::outputs(vector<Customer> &customers, vector<Purchase> &purchases, int billNumber, int customerID)
{
    output(billNumber, customerID);

    float cash = 0, sum = 0, qty = 0, sumt = 0;
    bool customerFound = false;

    for (int i = 0; i < totalitems; i++)
    {
        sumt += tprice[i];
        sum += vatprice[i];
        qty += Qty[i];
    }
    cout << "\nTotal:";
    cout << "\n------------------------------------------------------------------------------";
    cout << "\n\tQuantity= " << qty << "\t\t Sum= " << sumt << "\tWith Vat:" << sum;
    cout << "\n------------------------------------------------------------------------------";

    int inputID;
    while (!customerFound)
    {
        cout << "\n\nEnter customer ID (or skip, use 0): ";
        cin >> inputID;

        if (inputID == 0)
        {
            cout << "Skipping customer ID entry." << endl;
            break;
        }

        for (const Customer &customer : customers)
        {
            if (inputID == customer.customerID)
            {
                customerFound = true;
                cout << "Customer Name: " << customer.customerName << endl;
                cout << "Phone Number: " << customer.phoneNumber << endl;
                break;
            }
        }

        if (!customerFound)
        {
            cout << "Customer ID not found. Please enter a valid customer ID or 0 to skip." << endl;
        }
    }

pay:

    cout << "\n\n\t\t\t****PAYMENT SUMMARY****\n";
    cout << "\n\t\t\tTotal cash given: ";
    cin >> cash;

    if (cash >= sum)
        cout << "\n\t\t\tTotal cash repaid: " << cash - sum << '\n';
    else
    {
        cout << "\n\t\t\tCash given is less than the total amount!!!";
        goto pay;
    }

    vector<string> purchasedItems;
    for (int i = 0; i < totalitems; i++)
    {
        purchasedItems.push_back(getItemName(i));
    }

    Purchase purchase;
    purchase.input(billNumber, customerID, sum, purchasedItems);
    purchases.push_back(purchase);
}

int passwords()
{
    string inputPassword;
    string correctPassword = "saikott";

    cout << "\n\n\n\n\n\n\t\t\tENTER THE PASSWORD: ";
    cin >> inputPassword;

    if (inputPassword == correctPassword)
        return 1;
    else
        return 0;
}

void displayCustomerDetails(const vector<Customer> &customers)
{
    if (customers.empty())
    {
        cout << "No customer details found. Create a customer ID first." << endl;
        return;
    }

    cout << "Customer Details:" << endl;
    for (size_t i = 0; i < customers.size(); ++i)
    {
        cout << "Serial Number: " << i + 1 << endl;
        cout << "Customer ID: " << customers[i].customerID << endl;
        cout << "Customer Name: " << customers[i].customerName << endl;
        cout << "Phone Number: " << customers[i].phoneNumber << endl;
        cout << "-------------------------" << endl;
    }
    cout << "Press any key to continue . . ." << endl;
    cin.ignore();
    cin.get();
}

void displayPurchaseDetails(const vector<Purchase> &purchases, int customerID)
{
    cout << "Purchase Details for Customer ID: " << customerID << endl;
    for (const Purchase &purchase : purchases)
    {
        if (purchase.customerID == customerID)
        {
            cout << "Bill No.: " << purchase.billNo << endl;
            cout << "Total Amount: " << purchase.totalAmount << endl;
            cout << "Purchased Items:" << endl;
            for (const string &item : purchase.purchasedItems)
            {
                cout << "\t" << item << endl;
            }
            cout << "-------------------------" << endl;
        }
    }
    cout << "Press any key to continue . . ." << endl;
    cin.ignore();
    cin.get();
}

int main()
{
    vat obj;
    char opt, ch;
    int a = 1;
    ifstream fin;
    vector<Customer> customers;
    vector<Purchase> purchases;
    int customerID = 0;

    loadCustomerDetails(customers);

    a = passwords();
    if (!a)
    {
        for (int i = 0; i < 2; i++)
        {
            cout << "\nWrong password, try once more\n";
            if (passwords())
            {
                goto last;
            }
            else
            {
                cout << "\n\n\t\t\t all attempts failed.....";
                cout << "\n\n\n\t\t\t see you.................. ";
                exit(0);
            }
        }
        cout << "\t\t\t sorry, all attempts failed............. \n \t\t\tinactive";
    }
    else
    {
    last:;
        do
        {
        start:
            system("PAUSE");
            system("CLS");
            cout << "\n\n\t\t\t------------------------------";
            cout << "\n\t\t\tShop Billing System";
            cout << "\n\t\t\t------------------------------";
            cout << "\n\n\t\t\tWhat you want to do?";
            cout << "\n\t\t\t1.\tTo enter a new entry\n\t\t\t2.\tTo view previous entries\n\t\t\t3.\tCreate New ID for Customer\n\t\t\t4.\tDisplay Customer Details\n\t\t\t5.\tSearch Customer Purchase History by ID\n\t\t\t6.\tExit\n";
            cout << "\n\nEnter your option: ";
            cin >> opt;
            switch (opt)
            {
            case '1':
                // Option 1: Enter a new entry
                obj.vatcal();
                obj.outputs(customers, purchases, generateBillNumber(), customerID);
                goto start;
            case '2':
                // Option 2: View previous entries
                fin.open("HIS.TXT", ios::in);
                while (fin.get(ch))
                {
                    cout << ch;
                }
                fin.close();
                goto start;
            case '3':
                // Option 3: Create New ID for Customer
                if (customerID == 0)
                {
                    Customer newCustomer = createCustomer(customers);
                    cout << "New customer created with ID: " << newCustomer.customerID << endl;
                    customerID = newCustomer.customerID;
                    customers.push_back(newCustomer);
                    saveCustomerDetails(customers);
                }
                else
                {
                    cout << "Customer ID already created: " << customerID << endl;
                }
                break;
            case '4':
                // Option 4: Display Customer Details
                displayCustomerDetails(customers);
                break;
            case '5':
                // Option 5: Search Customer Purchase History by ID
                if (customers.empty())
                {
                    cout << "No customer IDs have been created. Please create a customer ID first." << endl;
                }
                else
                {
                    int searchID;
                    cout << "Enter Customer ID to search for purchase history: ";
                    cin >> searchID;
                    displayPurchaseDetails(purchases, searchID);
                }
                break;
            case '6':
                // Option 6: Exit
                exit(0);
            default:
                cout << "\a";
            }
        } while (opt != '6');
    }
    return 0;
}
