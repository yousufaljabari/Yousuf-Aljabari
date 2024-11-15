
#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsfileName = "Clients.txt";
struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
	bool MarkForUpdate = false;
};
enum enAtmMainMenu
{
	QuickWithDraw = 1,
	NormalWithDraw = 2,
	Depoist = 3,
	CheckBalance = 4,
	Logout = 5
};
double ReadNumberd(string message)
{
	double Number;
	cout << message << endl;
	cin >> Number;
	return Number;
}

int ReadNumber(int from,int to,string Message)
{
	int Choice;
	cout << Message << endl;
	cin >> Choice;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin >> Choice;
	}
	if (Choice<from || Choice>to)
	{
		cin.clear();
		cout <<"invalid Choice ,Enter a Valid One :  ";
		cin >> Choice;
	}
	
	return Choice;
}
vector<string>SplitString(string S1, string delim = "#//#")
{
	vector<string>VstoreWords;
	short pos = 0;
	string Sword;
	while ((pos = S1.find(delim)) != string::npos)
	{
		Sword = S1.substr(0, pos);
		if (Sword != " ")
		{
			VstoreWords.push_back(Sword);
		}
		S1.erase(0, pos + delim.length());
	}
	if (S1 != " ")
	{
		VstoreWords.push_back(S1);
	}
	return VstoreWords;
}
stClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	stClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to

	return Client;
}
string ConvertCLinettoLine(stClient Client, string Sperator = "#//#")
{
	string stClientline = "";
	stClientline += Client.AccountNumber + Sperator;
	stClientline += Client.PinCode + Sperator;
	stClientline += Client.Name + Sperator;
	stClientline += Client.Phone + Sperator;
	stClientline += to_string(Client.AccountBalance);

	return stClientline;
}
void fillFileUsingvClients(vector<stClient>&vClient)
{
	
	fstream MyFile;
	string Line;
	MyFile.open(ClientsfileName, ios::out);
	if (MyFile.is_open())
	{
		for (const stClient& c : vClient)
		{
			Line = ConvertCLinettoLine(c);
			MyFile << Line << endl;
		}
		MyFile.close();
	}
}

vector<stClient>LoadClientInfoFromFile(string FileName)
{
	vector<stClient>vClients;
	stClient Client;
	fstream MyFile;
	string Line;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}
bool CheckAccountNumberAndPinCodeifCorretOrNot(string& AccountNumber, string& Pincode, stClient& client)
{
	vector<stClient>vClients;
	vClients = LoadClientInfoFromFile(ClientsfileName);
	for (stClient& c : vClients)
	{
		if (AccountNumber ==c.AccountNumber && Pincode == c.PinCode)
		{
			client = c;
			return true;
		}
	}
	return false;
}
int getQuickWithDraw(int choice)
{
	

	switch ((choice))
	{
	case 1:
		return 20;
	
	case 2:
		return 50;
	case 3: 
		return 100;
	case 4:
		return 200;
	case 5 : 
		return 400;
	case 6 :
		return 600;
	case 7 : 
		return 800;
	case 8: 
		return 1000;
	case 9: 
		return 0;

	}
}

void QuickAddMenu(vector<stClient> &vClients,stClient& Client)
{
	int Choice = 0;
	int QuickWithDrawValue = 0;
	char Accept;

	do
	{
		cout << "=================================================================\n";
		cout << "		Quick WithDraw \n";
		cout << "=================================================================\n";
		cout << "    [1] 20        [2] 50\n";
		cout << "    [3] 100       [4] 200\n";
		cout << "    [5] 400       [6] 600\n";
		cout << "    [7] 800       [8] 1000\n";
		cout << "    [9] Exit \n";
		cout << "=================================================================\n";
		cout << "Your Balance is " << Client.AccountBalance << endl;
		Choice = ReadNumber(1,9,"Enter What to WithDraw from [1] to [9] ? ");
		if (Choice == 9)
			break;
		cout << "\n\nAre You Sure You Want Perform this Transaction?[Y/N]?";
		cin >> Accept;

		if (toupper(Accept) == 'Y')
		{
		    QuickWithDrawValue= getQuickWithDraw(Choice);
			if (QuickWithDrawValue > 0 && QuickWithDrawValue <= Client.AccountBalance)
			{
				Client.AccountBalance -= QuickWithDrawValue;

				for (stClient& c : vClients)
				{
					if (c.AccountNumber == Client.AccountNumber)
					{
						c.AccountBalance = Client.AccountBalance;
						break;
					}
				}

				cout << "Done Successfully. New Balance is : " << Client.AccountBalance << endl;
				system("pause>0");
				fillFileUsingvClients(vClients);
			}
			else if (QuickWithDrawValue > Client.AccountBalance)
			{
				cout << "Insufficient balance for this withdrawal.\n";
			}
		}
		system("cls");
	} while (Choice != 9);
}
bool AmountMultipleofFive(stClient & Client, vector<stClient>&vClients, int Number)
{
	Client.AccountBalance -= Number;
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == Client.AccountNumber)
		{
			C = Client;
			return true ;
		}
	}
	return false;
}
void NormalWithDrawScreen(vector<stClient>& vClients, stClient& Client)
{
	short Number;
	char Accept;
	cout << "=================================================================\n";
	cout << "        Normal WithDraw Screen \n";
	cout << "=================================================================\n";
	cout << "\n\nEnter An Amount Multiple of 5's ? ";
	cin >> Number;
	while (Number % 5 != 0)
	{
		cout << "\n\nEnter An Amount Multiple of 5's ? ";
		cin >> Number;
	}
	cout << "Are you sure you want to perform this transaction? [Y/N]: ";
	cin >> Accept;
	if (toupper(Accept) == 'Y')
	{
		
		if (AmountMultipleofFive(Client, vClients, Number))
		{
			cout << "\n\nDone Successfully. New Balance is : " << Client.AccountBalance << endl;
		}
		else
		{
			cout << "\n\nError: Client not found." << endl;
		}
	}
}
void printClient(stClient &Client)
{

	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << (Client.AccountBalance);

}
double DepoistMoney(stClient& Client)
{
	char ch;
	double AddMoney;
	cout << "\n";
	AddMoney = ReadNumberd("\nPlease Enter The Deposit Amount ?");
	cout << "\nAre You Sure You Want Perform This Transction ? [Y]/[N] ?";
	cin >> ch;
	if (toupper(ch) == 'Y')
	{
		Client.AccountBalance += AddMoney;
	}
	return Client.AccountBalance;
}
void StratDepoistScreen(vector<stClient>&VClient,stClient &Client)
{
	
	cout << "=================================================================\n";
	cout << "		Depoist Screen \n";
	cout << "=================================================================\n";
	cout << "\n";
	for (stClient& C : VClient)
	{
		if (C.AccountNumber == Client.AccountNumber)
		{
			C=Client;
			cout << "\n";
			printClient(Client);
			cout << "\n";
			Client.AccountBalance = DepoistMoney(Client);
			cout << "\n\n";
			cout << "Press Any Key To Go Back To Trasnction Menu ..\n";
			break;
		}
		
	}
	fillFileUsingvClients(VClient);
}
void StartAtmMainMenuScreen(int Choice, vector<stClient>&vClients,stClient &Client)
{


	switch (enAtmMainMenu(Choice))
	{
	case enAtmMainMenu::QuickWithDraw:
		system("cls");
		QuickAddMenu(vClients,Client);
		cout << "\nPlease Enter Any key to Go Back Main Menu\n";
		system("pause>0");
		system("cls");
		fillFileUsingvClients(vClients);
		break;
	case enAtmMainMenu::NormalWithDraw:
		system("cls");
		NormalWithDrawScreen(vClients,Client);
		cout << "\nPlease Enter Any key to Go Back Main Menu\n";
		system("pause>0");
		system("cls");
		fillFileUsingvClients(vClients);
		break;
	case enAtmMainMenu::Depoist:
		system("cls");
		StratDepoistScreen(vClients,Client);
		cout << "\nPlease Enter Any key to Go Back Main Menu\n";
		system("pause>0");
		system("cls");
		fillFileUsingvClients(vClients);
		break;
	case enAtmMainMenu::CheckBalance:
		system("cls");
		cout << "=================================================================\n";
		cout << "		Check Balance Screen\n";
		cout << "=================================================================\n";
		for (stClient& c : vClients)
		{
			if (c.AccountNumber == Client.AccountNumber)
			{
				c.AccountBalance = Client.AccountBalance;
				break;
			}
		}
		cout << "\nYour Balance is " << Client.AccountBalance << endl;
		cout << "\nPlease Enter Any key to Go Back Main Menu\n";
		system("pause>0");
		system("cls");
		fillFileUsingvClients(vClients);
		break;
	case enAtmMainMenu::Logout:
		break;
	}
	
}
void printAtmMainMenuScreen(vector<stClient>&vClients,stClient &Client)
{ 
	
	int Choice;
	do
	{
		cout << "=================================================================\n";
		cout << "		Atm Main Menu Screen  \n";
		cout << "=================================================================\n";
		cout << "\t\t[1]Quick WithDraw.\n";
		cout << "\t\t[2]Normal WithDraw.\n";
		cout << "\t\t[3]Depoist.\n";
		cout << "\t\t[4]Check Balance.\n";
		cout << "\t\t[5]Logout.\n";
		cout << "=================================================================\n";
		Choice = ReadNumber(1, 5, "Choose What Do You Want to do? [1 to 5 ] ? ");
		StartAtmMainMenuScreen(Choice,vClients, Client);
       	fillFileUsingvClients(vClients);
	} while (Choice != 5);
}
void login()
{
	vector<stClient>vClients;
	vClients = LoadClientInfoFromFile(ClientsfileName);
	cout << "----------------------------------------\n";
	cout << "	 Login Screen \n";
	cout << "----------------------------------------\n";
	string AccountNumber;
	string Pincode;
	stClient Client;
	cout << "Enter Account Number : ";
	getline(cin >> ws, AccountNumber);
	cout << "Enter Passeword : ";
	getline(cin >> ws, Pincode);
	if (CheckAccountNumberAndPinCodeifCorretOrNot(AccountNumber, Pincode, Client))
	{
		system("cls");
		printAtmMainMenuScreen(vClients,Client);
	}
	else
	{

		cout << "\n Client With [" << AccountNumber << "] and Pincode [" << Pincode << "] not found.\n";
		
		cout << "\n\nThat is invalid Account Number , Enter Another Account Number..  \n";
		system("pause>0");
		system("cls");
		login();
	}
}
int main()
{

	login();

}
