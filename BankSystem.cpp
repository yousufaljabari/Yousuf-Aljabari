#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "MyClients.txt";

int ReadChoice()
{
	int Choice;
	cout << "Choose What do you want to do? [1 to 6] ? ";
	cin >> Choice;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "invalid Choice , Enter a Vaild one " << endl;

		cin >> Choice;
	}
	return Choice;

}
double ReadNumber(string Message)
{
	double Number;
	cout << Message << endl;
	cin >> Number;
	return Number;
}
string  ReadString(string Message)
{
	string S1;
	cout << Message;
	getline(cin, S1);
	return S1;
}
enum enTransctions
{
	Deposit = 1,
	WithDraw = 2,
	TotaltBalnce = 3,
	MainMenu = 4

};
enum enMainMenuScreen
{
	showClientList = 1,
	addNewClient = 2,
	deleteClient = 3,
	updateClientInfo = 4,
	findClient = 5,
	Transctions = 6,
	Exit = 7
};
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
stClient ReadClientWithOutAccountNumber()
{
	stClient Client;
	cout << "Enter Pin Code : ";
	getline(cin >> ws, Client.PinCode);
	cout << "\n Enter Name : ";
	getline(cin, Client.Name);
	cout << "\nEnter Phone : ";
	getline(cin, Client.Phone);
	cout << "\nEnter Account Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}
void CheckSameClient(string& AccountNumber, vector<stClient>& VClient)
{
	for (stClient& c : VClient)
	{
		do
		{
			if (c.AccountNumber == AccountNumber)
			{
				cout << "Client With [" << AccountNumber << "] Already exists, Enter Another Account Number : ";
				getline(cin >> ws, AccountNumber);
			}
		} while (c.AccountNumber == AccountNumber);
	}
}
stClient ReadClient(vector<stClient>& VClient)
{
	stClient Client;

	cout << "Client Information : \n\n";

	cout << "\nEnter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);
	if (!VClient.empty())
	{
		CheckSameClient(Client.AccountNumber, VClient);
	}
	cout << "\nEnter PinCode : ";
	getline(cin, Client.PinCode);
	cout << "\nEnter Name : ";
	getline(cin, Client.Name);
	cout << "\nEnter Phone :  ";
	getline(cin, Client.Phone);
	cout << "\nEnter Account Balance : ";
	cin >> Client.AccountBalance;

	VClient.push_back(Client);
	return Client;
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
void FillFileUsingClient(string FileName, stClient Client)
{

	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	string Line;
	if (MyFile.is_open())
	{

		Line = ConvertCLinettoLine(Client);
		MyFile << Line << endl;
		MyFile.close();

	}
}
vector<stClient>SaveClientinVectorFromFile(string FileName)
{
	vector<stClient>vstClient;

	fstream MyFile;
	stClient Client;
	string line;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			Client = ConvertLinetoRecord(line);
			vstClient.push_back(Client);
		}
		MyFile.close();
	}
	return vstClient;

}
void AddClient(vector<stClient>& vClient)
{
	vector<string>vstring;
	stClient Client;
	Client = ReadClient(vClient);
	FillFileUsingClient(ClientsFileName, Client);
}
void AddClients(vector<stClient>vClient)
{
	char c;
	do
	{
		system("cls");
		cout << "Enter New Client : \n\n";
		vClient = SaveClientinVectorFromFile(ClientsFileName);
		AddClient(vClient);
		cout << "\n\n Do You Want Add a new Client ? [Y]/[N] ";
		cin >> c;
	} while (toupper(c) == 'Y');
}
void printClient(stClient Client)
{

	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << (Client.AccountBalance);

}
void printClients(vector<stClient>& VClient)
{


	VClient = SaveClientinVectorFromFile(ClientsFileName);

	cout << "\t\t\t\t\tClient List(" << VClient.size() << ") Client (S)\n";
	cout << "-------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(15) << "Account Number ";
	cout << "| " << left << setw(10) << "Pin Code ";
	cout << "| " << left << setw(40) << " Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n-------------------------------------------------------------------------------------------------\n";
	if (VClient.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	{
		for (stClient& C : VClient)
		{
			printClient(C);
			cout << endl;
		}
	}
	cout << "\n-------------------------------------------------------------------------------------------------\n";

}
bool FindClientByAccountNumber(string AccountNumber, vector <stClient>& vclients, stClient& Client)
{
	for (stClient& C : vclients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vclients)
{
	for (stClient& C : vclients)


		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	return false;
}
vector<stClient>LoadCleinrtsDataToFile(string FileName, vector<stClient>& Vclient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string line;
	if (MyFile.is_open())
	{
		for (stClient& C : Vclient)
		{
			if (C.MarkForDelete == false)
			{
				line = ConvertCLinettoLine(C);
				MyFile << line << endl;
			}
		}
		MyFile.close();
	}
	return Vclient;

}
bool DeleteClientByAccountNumber(vector <stClient>& vclients, string AccountNumber)
{

	stClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vclients, Client))
	{
		printClient(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? "; cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vclients);
			LoadCleinrtsDataToFile(ClientsFileName, vclients);
			//Refresh Clients
			vclients = SaveClientinVectorFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	}
	return false;
}
bool MarkClientforUpdateClient(string AccountNumber, vector<stClient>& Vclient)
{
	for (stClient& Client : Vclient)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.MarkForUpdate = true;
			return true;
		}
	}
	return false;
}
vector<stClient>LoadCleinrtsDataToFileForUpdate(string FileName, vector<stClient>& VClient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string line;
	if (MyFile.is_open())
	{
		for (stClient& C : VClient)
		{
			if (C.MarkForUpdate == false)
			{
				line = ConvertCLinettoLine(C);
				MyFile << line << endl;
			}
		}
		MyFile.close();
	}
	return VClient;
}
void updateCLient(vector<stClient>& vClient, string AccountNumber)
{

	stClient Client;
	char c = 'N';
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		printClient(Client);
		cout << "Are You Sure You Want Update this Client ? [Y]/[N] ";
		cin >> c;
		if (toupper(c) == 'Y')
		{
			for (stClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ReadClientWithOutAccountNumber();
					C.AccountNumber = AccountNumber;
					break;
				}
			}
			LoadCleinrtsDataToFileForUpdate(ClientsFileName, vClient);
			cout << "Clinet Updated Successfully." << endl;
		}

	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") Not Found .. " << endl;
	}
}
double DepoistMoney(stClient& Client)
{
	char ch;
	double AddMoney;
	cout << "\n";
	AddMoney = ReadNumber("\nPlease Enter The Deposit Amount ?");
	cout << "\nAre You Sure You Want Perform This Transction ? [Y]/[N] ?";
	cin >> ch;
	if (toupper(ch) == 'Y')
	{
		Client.AccountBalance += AddMoney;
	}
	return Client.AccountBalance;
}
double WithDrawMoney(stClient& Client)
{
	char ch;
	double SubMoney;
	cout << "\n";
	SubMoney = ReadNumber("\nPlease Enter WithDraw Amount ?");
	if (SubMoney > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds The Balance , You Can WithDraw Up To : " << Client.AccountBalance << endl;
		SubMoney = ReadNumber("\nPlease Enter Another Amount ?");
	}
	cout << "\nAre You Sure You Want Perform This Transction ? [Y]/[N] ?";
	cin >> ch;
	if (toupper(ch) == 'Y')
	{
		Client.AccountBalance += (-1 * SubMoney);
	}
	return Client.AccountBalance;
}
void StratDepoistScreen(vector<stClient>& VClient)
{
	string AccountNumber = "";
	stClient Client;
	bool clientFound = false;


	cout << "Please Enter Account Number: ";
	getline(cin >> ws, AccountNumber);
	cout << "\n";

	for (stClient& C : VClient)
	{
		if (C.AccountNumber == AccountNumber)
		{

			cout << "\n";
			printClient(C);
			cout << "\n";
			C.AccountBalance = DepoistMoney(C);
			cout << "\n\n";
			cout << "Press Any Key To Go Back To Trasnction Menu ..\n";
			clientFound = true;
			break;
		}
	}

	if (!clientFound)
	{
		cout << "\n\nClient with Account Number [" << AccountNumber << "] does not exist.\n\n";

		StratDepoistScreen(VClient);
		cout << endl;
	}


	LoadCleinrtsDataToFile(ClientsFileName, VClient);

}
void StratWithDrwaScreen(vector<stClient>& VClient)
{
	string AccountNumber = "";
	stClient Client;
	bool clientFound = false;


	cout << "Please Enter Account Number: ";
	getline(cin >> ws, AccountNumber);


	for (stClient& C : VClient)
	{
		if (C.AccountNumber == AccountNumber)
		{

			cout << "\n";
			printClient(C);
			cout << "\n\n";
			C.AccountBalance = WithDrawMoney(C);
			cout << "\n\n";
			cout << "Press Any Key To Go Back To Trasnction Menu ..\n";
			clientFound = true;
			break;
		}
	}

	if (!clientFound)
	{
		cout << "\n\nClient with Account Number [" << AccountNumber << "] does not exist.\n";
		StratWithDrwaScreen(VClient);

	}


	LoadCleinrtsDataToFile(ClientsFileName, VClient);


}
void printClientBalance(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << (Client.AccountBalance);
}
void printTotalBalance(vector<stClient>& VClient)
{
	int SumofAllBalance = 0;

	VClient = SaveClientinVectorFromFile(ClientsFileName);
	cout << "\t\t\t\t\tClient List(" << VClient.size() << ") Client (S)\n";
	cout << "-------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(15) << "Account Number ";
	cout << "| " << left << setw(40) << " Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n-------------------------------------------------------------------------------------------------\n";
	if (VClient.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!\n";
	else
	{
		for (stClient& C : VClient)
		{
			SumofAllBalance += C.AccountBalance;
			printClientBalance(C);
			cout << endl;
		}
	}
	cout << "-------------------------------------------------------------------------------------------------\n";
	cout << "\t\t\t\t\t Total Balance = " << SumofAllBalance << endl;
	cout << "\n\nPress Any Key To Go Back To Trasnction Menu ..\n";
}
void stratTransCtionsMenu(int Choice, vector<stClient>& VClient)
{

	switch ((enTransctions)Choice)
	{
	case enTransctions::Deposit:
	{
		system("cls");
		StratDepoistScreen(VClient);
		system("pause>0");
		break;
	}
	case enTransctions::WithDraw:
	{
		system("cls");
		StratWithDrwaScreen(VClient);
		system("pause>0");
		break;
	}
	case enTransctions::TotaltBalnce:
	{
		system("cls");
		printTotalBalance(VClient);
		system("pause>0");
		break;
	}
	case enTransctions::MainMenu:
		system("cls");
		break;
	}
}
void printTransactionsMenu(vector<stClient>& VClient)
{
	int choice;
	do
	{
		system("cls");
		cout << "==========================================\n";
		cout << "\t\tTransctions Menue Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] Deposit .\n";
		cout << "\t[2] WithDraw.\n";
		cout << "\t[3] Total Balance.\n";
		cout << "\t[4] Main Menue .\n";
		cout << "===========================================\n";
		cout << "Choose What Do You Want To Do ? [1 to 4 ] ?";
		cin >> choice;
		stratTransCtionsMenu(choice, VClient);
	} while (choice != 4);
}

void startMainMenueScreen(int Choice, vector<stClient>& VClients)
{
	string AccountNumber = "";
	stClient Client;
	system("cls");
	switch ((enMainMenuScreen)Choice)
	{

	case enMainMenuScreen::showClientList:

		printClients(VClients);
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause");
		system("cls");
		break;

	case enMainMenuScreen::addNewClient:

		AddClients(VClients);
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause>0");
		system("cls");
		break;

	case enMainMenuScreen::deleteClient:

		cout << " please Enter Account Number for Delete : ";
		getline(cin >> ws, AccountNumber);
		DeleteClientByAccountNumber(VClients, AccountNumber);
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause>0");
		system("cls");
		break;


	case enMainMenuScreen::updateClientInfo:

		cout << " please Enter Account Number for Update : ";
		getline(cin >> ws, AccountNumber);
		updateCLient(VClients, AccountNumber);
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause>0");
		system("cls");
		break;


	case enMainMenuScreen::findClient:
		cout << " please Enter Account Number for Find : ";
		getline(cin >> ws, AccountNumber);
		if (FindClientByAccountNumber(AccountNumber, VClients, Client))
			printClient(Client);
		else
			cout << "Cleint With Account Number(" << AccountNumber << ") Not Found ...";
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause>0");
		system("cls");
		break;

	case enMainMenuScreen::Transctions:

		system("cls");
		printTransactionsMenu(VClients);
		system("pause");
		system("cls");
		break;

	case enMainMenuScreen::Exit:
		cout << "\n======================================\n\n";
		cout << "\tProgrm Ended ...";
		cout << "\n\n======================================\n";
		break;


	}
}

void printMainMenu(vector<stClient>& VClient)
{
	int choice;
	do
	{
		cout << "======================================================\n";
		cout << "\t\tMain Menu Screen" << endl;
		cout << "======================================================\n";
		cout << left << "[1] Show Client List .\n";
		cout << left << "[2] Add New Client .\n";
		cout << left << "[3] Delete Client .\n";
		cout << left << "[4] Update Client .\n";
		cout << left << "[5] Find Client .\n";
		cout << left << "[6] Transction .\n";
		cout << left << "[7] Exit .\n\n";
		cout << "=======================================================\n";
		choice = ReadChoice();
		startMainMenueScreen(choice, VClient);
	} while (choice != 7 || choice > 7);
	VClient = SaveClientinVectorFromFile(ClientsFileName);
}
int main()
{
	vector<stClient> VClient;
	VClient = SaveClientinVectorFromFile(ClientsFileName);
	printMainMenu(VClient);

}
