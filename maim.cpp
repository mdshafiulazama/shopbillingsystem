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

    void input(int billNumber, int custID, float amount)
    {
        billNo = billNumber;
        customerID = custID;
        totalAmount = amount;
    }
};

class head
{
    char Iname[50][50];

public:
    int totalitems;
    float Qty[3];
    float price[3];
    int vatprice[3];
    int tprice[3];
    void input();
    void output(int billNumber, int customerID);
};

class vat : public head
{
    float vats;
    vector<Customer>& customers;

public:
    vat(vector<Customer>& cust) : customers(cust) {}
    void vatcal();
    void outputs(int billNumber, int customerID);
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

const Customer* findCustomerByID(const vector<Customer>& customers, int customerID) {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].customerID == customerID) {
            return &customers[i];
        }
    }
    return nullptr; // Customer not found
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

// Add this function to display all purchase details for a specific customer
void displayAllCustomerDetails(const vector<Customer>& customers, int customerID)
{
    const Customer* foundCustomer = findCustomerByID(customers, customerID);
    if (foundCustomer != nullptr) {
        cout << "Customer found:" << endl;
        cout << "Customer ID: " << foundCustomer->customerID << endl;
        cout << "Customer Name: " << foundCustomer->customerName << endl;
        cout << "Phone Number: " << foundCustomer->phoneNumber << endl;

        ifstream purchaseFile("HIS.TXT");
        if (purchaseFile)
        {
            string line;
            bool found = false;

            while (getline(purchaseFile, line))
            {
                if (line.find("Bill No.: ") != string::npos)
                {
                    int billNumber = stoi(line.substr(line.find(" ") + 1));
                    getline(purchaseFile, line); // Read and ignore the "Customer ID" line
                    int fileCustomerID;
                    purchaseFile >> fileCustomerID;
                    if (fileCustomerID == customerID)
                    {
                        found = true;
                        cout << line << endl; // Print "Customer ID" line
                        getline(purchaseFile, line); // Read and ignore the dashed line
                        while (getline(purchaseFile, line) && !line.empty())
                        {
                            cout << line << endl; // Print the purchase details
                        }
                    }
                }
            }

            if (!found)
            {
                cout << "No purchase history found for Customer ID: " << customerID << endl;
            }

            purchaseFile.close();
        }
        else
        {
            cout << "Unable to open purchase history file." << endl;
        }
    } else {
        cout << "Customer not found." << endl;
    }
}

void head::input()
{
    system("CLS");
    cout << "\nEnter number of items= ";
    cin >> totalitems;

    for (int i = 0; i < totalitems; i++)
    {
        cout << "\nEnter name of item " << i + 1 << ": ";
        cin >> Iname[i];
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

void vat::outputs(int billNumber, int customerID)
{
    output(billNumber, customerID);

    float cash = 0, sum = 0, qty = 0, sumt = 0;

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
    cout << "\n\nEnter customer ID: ";
    cin >> inputID;

    const Customer* foundCustomer = findCustomerByID(customers, inputID);
    if (foundCustomer != nullptr)
    {
        cout << "Customer found:" << endl;
        cout << "Customer ID: " << foundCustomer->customerID << endl;
        cout << "Customer Name: " << foundCustomer->customerName << endl;
        cout << "Phone Number: " << foundCustomer->phoneNumber << endl;

        displayAllCustomerDetails(customers, inputID); // Display all purchase details
    }
    else
    {
        cout << "Customer not found." << endl;
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
}

int passwords()
{
    string inputPassword;
    string correctPassword = "saikott";  // Replace with your actual password

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

int main()
{
    vector<Customer> customers;
    vat obj(customers);
    char opt, ch;
    int a = 1;
    ifstream fin;
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
        const Customer* searchCustomer = nullptr;
        do
        {
        start:
            system("PAUSE");
            system("CLS");
            cout << "\n\n\t\t\t------------------------------";
            cout << "\n\t\t\tShop Billing System";
            cout << "\n\t\t\t------------------------------";
            cout << "\n\n\t\t\tWhat you want to do?";
            cout << "\n\t\t\t1.\tTo enter a new entry\n\t\t\t2.\tTo view previous entries\n\t\t\t3.\tCreate New ID for Customer\n\t\t\t4.\tDisplay Customer Details\n\t\t\t5.\tSearch Customer by ID\n\t\t\t6.\tExit\n";
            cout << "\n\nEnter your option: ";
            cin >> opt;
            switch (opt)
            {
            case '1':
                if (customerID == 0)
                {
                    cout << "Please create a customer ID first." << endl;
                    break;
                }
                obj.vatcal();
                obj.outputs(generateBillNumber(), customerID);
                goto start;
            case '2':
                fin.open("HIS.TXT", ios::in);
                while (fin.get(ch))
                {
                    cout << ch;
                }
                fin.close();
                goto start;
            case '3':
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
                displayCustomerDetails(customers);
                break;
            case '5':
                int searchID;
                cout << "Enter customer ID to search: ";
                cin >> searchID;
                displayAllCustomerDetails(customers, searchID);
                break;
            case '6':
                exit(0);
            default:
                cout << "\a";
            }
        } while (opt != '6');
    }
    return 0;
}
