#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,
    eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8
};
enum enManageUsersMenueOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};
enum enMainMenuePermissions {
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClient = 8, pFindClient = 16, pTransactionsMenue = 32, pManageUsers = 64
};
const string ClientsFileName = "Clients.txt";
const string UserFileName = "Users.txt";


void Login();
void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenueScreen();
bool CheckAccessPermission(enMainMenuePermissions Permission);

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
struct sUser
{
	string Username;
	string Password;
    int Permissions;
	bool MarkForDelete = false;
};  
sUser CurrentUser;
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; 

 
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); 
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);


    return Client;

}
sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{

    sUser User;
    vector<string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;

}


string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
string ConvertUserRecordToLine(sUser User, string Seperator = "#//#")
{

    string stUserRecord = "";

    stUserRecord += User.Username + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}
bool UserExistsByUsername(string Username, string FileName)
{

    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertUserLinetoRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }


            vUsers.push_back(User);
        }

        MyFile.close();

    }

    return false;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
        return true;
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}
sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

int ReadPermissionsToSet()
{
    int permissions = 0;
    char choice = 'n';

    cout << "\nDo you want to give this user Full Access? y/n? ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        return -1;
    }
    cout << "\nDo You Want To Give Access To :\n";

    cout << "\nShow Client List? y/n? ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pListClients;
    }
    cout << "\nAdd New Client? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pAddNewClient;
    }
    cout << "\nDelete Client? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pDeleteClient;
    }
    cout << "\nUpdate Client? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pUpdateClient;
    }
    cout << "\nFind Client? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pFindClient;
    }
    cout << "\nTransactions Menue? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pTransactionsMenue;
    }
    cout << "\nManage Users Menue? y/n? ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        permissions += enMainMenuePermissions::pManageUsers;
    }
    return permissions;
}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username? ";


    getline(cin >> ws, User.Username);

    while (UserExistsByUsername(User.Username, UserFileName))
    {
        cout << "\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }


    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();



    return User;

}

vector <sUser> LoadUsersDataFromFile(string FileName)
{

    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sUser Users;

        while (getline(MyFile, Line))
        {

            Users = ConvertUserLinetoRecord(Line);

            vUsers.push_back(Users);
        }

        MyFile.close();

    }

    return vUsers;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
void ShowAccessDeniedMessage()
{
    cout <<"\n-----------------------------------\n";
    cout << "\nAccess Denied, \nYou Dont Have Permission To Do This,\nPleas Conact Your Admin,";
    cout << "\n-----------------------------------\n";
}
void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintUsersRecordLine(sUser User)
{

    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedMessage();



        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________";
    cout << "______________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n___________________________________________________________";
    cout << "_____________________________________\n" << endl;
    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }
    cout << "\n___________________________________________________________";
    cout << "______________________________________\n" << endl;

}
void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________";
    cout << "____________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n___________________________________________________________";
    cout << "____________________________\n" << endl;
    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n___________________________________________________________";
    cout << "____________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient & Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>&vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
       
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();



        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}
void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();



        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();



        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();



        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

   
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}
void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenueScreen();

}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadManageUsersMenueOption()
{
    short choise = 0;
    cout << "\nChoose What Do You Want To Do? [1 to 6]? ";
    cin >> choise;

    return choise;
}


bool FindUserByUserNameAndPassword(string UserName, string Password, sUser& USer)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
    for (sUser U : vUsers)
    {

        if (U.Username == UserName && U.Password == Password)
        {
            USer = U;
            return true;
        }
        
    }
    return false;
}
string ReadUserName()
{
    string username;

    cout << "\nEnter UserName? ";
    cin >> username;
    return username;

}
string ReadPassword()
{
    string password;

    cout << "\nEnter Password? ";
    cin >> password;

    return password;
}
void ShowListUsersScreen()
{


    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n___________________________________________________________";
    cout << "______________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n___________________________________________________________";
    cout << "_____________________________________\n" << endl;
    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (sUser User : vUsers)
        {

            PrintUsersRecordLine(User);
            cout << endl;
        }
    cout << "\n___________________________________________________________";
    cout << "______________________________________\n" << endl;

}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UserFileName, ConvertUserRecordToLine(User));

}
void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {

        cout << "Adding New User:\n\n";

        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more users? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}
void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();

}
bool MarkUserForDeleteByUsername(string Username, vector <sUser>& vUsers)
{

    for (sUser& U : vUsers)
    {

        if (U.Username == Username)
        {
            U.MarkForDelete = true;
            return true;
        }

    }

    return false;
}
void PrintUserCard(sUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername: " << User.Username;
    cout << "\nPassword: " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n-----------------------------------\n";

}
bool FindUserByUsername(string Username, vector <sUser> vUsers, sUser& User)
{

    for (sUser &U : vUsers)
    {

        if (U.Username == Username)
        {
            User = U;
            return true;
        }

    }
    return false;

}
vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open())
    {

        for (sUser U : vUsers)
        {

            if (U.MarkForDelete == false)
            {

                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;
}

bool DeleteUserByUsername(string Username, vector <sUser>& vUsers)
{

    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        if (Username == "Admin")
        {
            cout << "\nYou Cannot Delete This User.\n";
            GoBackToManageUsersMenue();
            return false;
        }
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UserFileName, vUsers);

            vUsers = LoadUsersDataFromFile(UserFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
        return false;
    }
}
void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
    string Username = ReadUserName();
    DeleteUserByUsername(Username, vUsers);

}
sUser ChangeUserRecord(string Username)
{
    sUser User;

    User.Username = Username;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();


    return User;
}
bool UpdateUserByUsername(string Username, vector <sUser>& vUsers)
{

   sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username,vUsers, User))
    {

        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sUser& U: vUsers)
            {
                if (U.Username == Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }

            }

            SaveUsersDataToFile(UserFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
        return false;
    }

}
void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
    string Username = ReadUserName();
    UpdateUserByUsername(Username, vUsers);

}
void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
    sUser User;
    string Username =ReadUserName();
    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
        cout << "\nUser with Username[" << Username << "] is not found!";

}

void PerformManageUsersMenueOptions(enManageUsersMenueOptions ManageUsersMenueOption)
{
    switch (ManageUsersMenueOption)
    {
    case enManageUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eAddNewUser:
    {
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eUpdateUser:
    {

        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eFindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eMainMenue:
        system("cls");
        ShowMainMenue();

        break;
    }

}
void ShowManageUsersMenueScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();


        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerformManageUsersMenueOptions((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}
void PerfromTransactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTransactionsMenue))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();


        return;
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;

        case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenueScreen();
        break;

        case enMainMenueOptions::eLogout:
            Login();
            break;
    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}
bool LoadUserInfo(string UserName, string password)
{
    if (FindUserByUserNameAndPassword(UserName, password, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFailed = false;

	string UserName, password;
    do
    {
        system("cls");

        cout << "===========================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "===========================================\n";

        if (LoginFailed)
        {

			cout << "\nInvalid UserName/Password!\n";
        }
        
		cout << "\nPlease enter UserName? ";
		cin >> UserName;

		cout << "\nPlease enter Password? ";
		cin >> password;

		LoginFailed = !LoadUserInfo(UserName, password);
    } while (LoginFailed);
    
    ShowMainMenue();

}
int main()

{
   Login();

    system("pause>0");
    return 0;
}
