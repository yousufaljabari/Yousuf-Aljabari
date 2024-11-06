#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "MyClients.txt";
const string UserFileName = "UsersFile1.txt";
void NotAccsesPerMissions();
struct stUser
{
	string UserName="";
	string PassWord="";
	int Permissions=0;
	
	bool DeleteFlag = false;
	bool UpdateFlag = false;

};


enum enMainMenueOptions {
	eAll=-1,eListClients = 1, eAddNewClient = 2, eDeleteClient = 4,
	eUpdateClient = 8, eFindClient = 16, eShowTransactionsMenue =32,
	eManageUsers = 64, eExit = 8
};
enum enUserMenuScreen
{

	ListUser=1,
	AddNewUser=2,
	DeleteUser=3,
	UpdateUsers=4,
	FindUser=5,
	mainMenuScreen=6

};
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
	magageusers = 7,
	Logout=8,
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
stUser GeneralUser;
int ReadChoice(short N1,short N2)
{
	int Choice;
	cout << "Choose What do you want to do?  ["<<N1<<"] to ["<<N2<<"] : ";
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
bool CheckAccessPermission(enMainMenueOptions Permission)
{
	if (GeneralUser.Permissions == enMainMenueOptions::eAll)
		return true;

	return((Permission & GeneralUser.Permissions) != 0);
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

	stClient Client;
	Client = ReadClient(vClient);
	FillFileUsingClient(ClientsFileName, Client);
}
void AddClients(vector<stClient>vClient)
{
	if (!CheckAccessPermission(enMainMenueOptions::eAddNewClient))
	{
		NotAccsesPerMissions();
		return;
	}

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

	if (!CheckAccessPermission(enMainMenueOptions::eListClients))
	{
		NotAccsesPerMissions();
		return;
	}

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
bool DeleteClientByAccountNumber(vector <stClient>& vclients)
{
	if (!CheckAccessPermission(enMainMenueOptions::eDeleteClient))
	{
		NotAccsesPerMissions();
		return 0;
	}

	stClient Client;
	string AccountNumber = "";
	cout << " please Enter Account Number for Delete : ";

	getline(cin >> ws, AccountNumber);
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
void updateCLient(vector<stClient>& vClient)
{

	if (!CheckAccessPermission(enMainMenueOptions::eUpdateClient))
	{
		NotAccsesPerMissions();
		return;
	}
	stClient Client;
	string AccountNumber = "";
	cout << " please Enter Account Number for Update : ";
	getline(cin >> ws, AccountNumber);
	cout << "\n\n";
	char c = 'N';
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		printClient(Client);
		cout << "\n\nAre You Sure You Want Update this Client ? [Y]/[N] ";
		cin >> c;
		cout << "\n\n";
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
			cout << "Clnet Updated Successfully." << endl;
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
	if (!CheckAccessPermission(enMainMenueOptions::eShowTransactionsMenue))
	{
		NotAccsesPerMissions();
		return;
	}
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
	return;
}

int  givingAccessPermissions()
{
	
	int Permissions = 0;
	char AcceptPermission;
	cout << "Do You Want to give access to :  \n";
	
	cout << "\nShow Client List? [Y]/[N]? ";
	cin >> AcceptPermission;
	
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eListClients;	
	}
	
	
	cout << "\n\nAdd New Client? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eAddNewClient;
		
	}

	
	cout << "\n\nDelete Client? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission)=='Y')
	{
		Permissions += enMainMenueOptions::eDeleteClient;
	}
	
	
	cout << "\n\nUpdate Client? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eUpdateClient;
	}


	cout << "\n\nFind Client? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eFindClient;
		
	}

	cout << "\n\nTransaction? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eShowTransactionsMenue;
		
	}


	cout << "\n\nManage Users? [Y]/[N]? ";
	cin >> AcceptPermission;
	if (toupper(AcceptPermission) == 'Y')
	{
		Permissions += enMainMenueOptions::eManageUsers;
	}

	return Permissions;

 }
stUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
	stUser User;
	vector<string> vUserData;
	vUserData = SplitString(Line, Seperator);
	if (vUserData.size() >= 3)
	{
		User.UserName = vUserData[0];
		User.PassWord = vUserData[1];
		User.Permissions= stoi(vUserData[2]);
	}
	return User;
}
vector<stUser>LoadUsersinVectorFromFile()
{
	vector<stUser>vStUsers;

	fstream UserFile;
	stUser User;
	string line;
	UserFile.open(UserFileName, ios::in);
	if (UserFile.is_open())
	{
		while (getline(UserFile, line))
		{
			 User= ConvertUserLinetoRecord(line);
			 vStUsers.push_back(User);
		}
		UserFile.close();
	}
	return vStUsers;

}

stUser ReadUserNameAndPassword()
{
	stUser User;
	cout << "\nEnter Username ?";
	getline(cin >> ws, User.UserName);
	cout << "\nEnter Password ? ";
	getline(cin, User.PassWord);
	return User;

}

int ReadPermission()
{
	int Permission=0;
	char AcceptPermission;
	cout << "Do You want to give full access ?[Y]/[N] ";
	cin >> AcceptPermission;
	if (tolower(AcceptPermission) == 'y')
	{
		Permission = -1;
	}
	else
	{
		Permission=givingAccessPermissions();

	}
	return Permission;

}
stUser ReadPasswordAndPermissons()
{
	stUser User;
	cout << "\nEnter Password ? ";
	getline(cin>>ws, User.PassWord);
	User.Permissions = ReadPermission();
	return User;
}
void checkSamePassword(string & Password ,vector<stUser>&vUsers)
{
	for(stUser &User:vUsers)
	do
	{
		if (Password == User.PassWord)
		{
			cout << "\n\nPassword With [" << Password << "] already Exist ,Enter another Password?";
			getline(cin, Password);
		}
	} while (Password == User.PassWord);
}
void checkSameUsers(string &UserName,vector<stUser>&vUsers)
{
	for (stUser& U : vUsers)
	{
		do
		{
			if (UserName == U.UserName)
			{
				cout << "\n\nUser With[" << UserName << "] already Exist, Enter another Username ? ";
				getline(cin >> ws, UserName);
			}
		} while (UserName == U.UserName);
		
	}
}

stUser ReadUserInfo(vector<stUser>&vUser)
{
	vUser = LoadUsersinVectorFromFile();
	stUser User;
	cout << "Enter Username ?";
	getline(cin >> ws, User.UserName);
	if (!vUser.empty())
		checkSameUsers(User.UserName, vUser);
	
	
	cout << "Enter Password ? ";
	getline(cin, User.PassWord);
	if (!vUser.empty())
		checkSamePassword(User.PassWord, vUser);
	User.Permissions = ReadPermission();
	return User;
}

string ConvertUserInfotoLine(stUser User,string Sperator="#//#")
{
	string Line="";
	Line += User.UserName + Sperator;
	Line += User.PassWord + Sperator;
	Line += to_string(User.Permissions) ;
	return Line;

}

void AddUserInfotoFile(stUser &User)
{
	fstream UserFile;
	UserFile.open(UserFileName,ios::out|ios::app);
	string Line;
	if (UserFile.is_open())
	{
		Line = ConvertUserInfotoLine(User);
		UserFile << Line << endl;
		UserFile.close();
	}

}
void printUser(stUser &User)
{
	cout << "| " <<setw(10)<< left << User.UserName ;
	cout << "| " << setw(10) << left << User.PassWord ;
	cout << "| " << setw(12) << left <<(User.Permissions);
	cout << endl;
}
void printUsers()
{
	vector<stUser>vUser;
	vUser = LoadUsersinVectorFromFile();
	cout << "			User List (" << vUser.size() << ") User (s)" << endl;
	cout << "-----------------------------------------------------------------------------------------------\n";
	cout<<setw(12) << left << "| User Name"  ;
	cout<<setw(12) << left << "| Password"   ;
	cout<<setw(12) << left << "| Permissions";
	cout << "\n-----------------------------------------------------------------------------------------------\n";
	for (stUser& User : vUser)
	{
		printUser(User);
	}
	cout << "\n-----------------------------------------------------------------------------------------------\n";
}
void AddUser()
{
	vector<stUser>VUsers;
	stUser User=ReadUserInfo(VUsers);
	AddUserInfotoFile(User);
}
void AddsUsers()
{

	vector<stUser>Users;
	char AddMore = 'y';
	cout << "	Add New User Screen \n";
	cout << "---------------------------------\n\n";
	do
	{
		system("cls");
		cout << "Adding New User : \n\n";

		Users = LoadUsersinVectorFromFile();
		AddUser();

		cout << "\n\nUser Add Successfuly, Do You Want Add To Add More Users? [Y]/[N]?";
		cin >> AddMore;

	} while (tolower(AddMore) == ('y'));
}
bool FindUserUsingUserName(string &UserName,stUser &User)
{
	vector<stUser>vUser;
	vUser = LoadUsersinVectorFromFile();
	for (stUser& U : vUser)
	{
		if (UserName == U.UserName)
		{
			User.UserName = UserName;
			User.PassWord = U.PassWord;
			User.Permissions = U.Permissions;
			
			return true;
		}
	}
	return false;
}
bool FindUserUsingPassword(string Password)
{
vector<stUser>vUser;
vUser = LoadUsersinVectorFromFile();
for (stUser& U : vUser)
{
	if (Password == U.PassWord)
	{
		return true;
	}
}
return false;
}
bool MarkforDeleteUser(string UserName, vector<stUser>&vUsers)
{
	
	vUsers = LoadUsersinVectorFromFile();
	for (stUser &U : vUsers)
	{
		if (U.UserName == UserName)
		{
			U.DeleteFlag = true;
			return true;
		}
	}
	return false;
}
void AddUserinfoToFileAftetFlagDelete(vector<stUser>& vUser)
{

	fstream UserFile;
	UserFile.open(UserFileName, ios::out);
	string Line;
	if (UserFile.is_open())
	{
		for (stUser& U : vUser)
		{
			if (U.DeleteFlag == false)
			{
				Line = ConvertUserInfotoLine(U);
				UserFile << Line << endl;
			}
		}
		UserFile.close();
	}
}

void DeleteUserinMenu()
{
	vector<stUser>vUsers;
	
	stUser User;
	string UserName;
	char DeleteUser;
	
		cout << "Please Enter UserName : ";
		getline(cin>>ws, UserName);
		if (FindUserUsingUserName(UserName,User))
		{
			cout << "\n\nThe Following are the client details : \n";
			printUser(User);
			cout << "\n\nDo You Want  Delete User ? ";
			cin >> DeleteUser;
			if (toupper(DeleteUser) == 'Y')
			{
				MarkforDeleteUser(UserName,vUsers);
				AddUserinfoToFileAftetFlagDelete(vUsers);
				vUsers = LoadUsersinVectorFromFile();
				cout << "User Deleted successfully \n";
			}
				

		}
		else
		{
			cout << "\nUser with [" << UserName << "]Not Found ...\n\n";
		}
	

}
bool MarkForUpdateUser(string UserName,vector<stUser>&vUser)
{
	vUser = LoadUsersinVectorFromFile();
	for (stUser& User : vUser)
	{
		if (User.UserName == UserName)
		{
			User.UpdateFlag = true;
			return true;
		}
	}
	return false;

}
void AddUserInformationtoFileByUpdateFlag(vector<stUser>&vUser)
{

	fstream UserFile;
	UserFile.open(UserFileName, ios::out);
	string Line;
	if (UserFile.is_open())
	{
		for (stUser& U : vUser)
		{
			if (U.UpdateFlag == false)
			{
				Line = ConvertUserInfotoLine(U);
				UserFile << Line << endl;
			}
		}
		UserFile.close();
	}
}
void UpdateUserMenu()
{
	vector<stUser>vUser;
	stUser User;
	string UserName;
	char UpdateUser;

	cout << "Please Enter UserName : ";
	getline(cin >> ws, UserName);
	if (FindUserUsingUserName(UserName, User))
	{
		cout << "\n\nThe Following are the client details : \n";
		cout << "\n--------------------------------------\n";
		printUser(User);
		cout << "\n--------------------------------------";
		cout << "\n\nDo You Want Update User ? ";
		cin >> UpdateUser;
		if (toupper(UpdateUser) == 'Y')
		{
			MarkForUpdateUser(UserName, vUser);
			for (stUser& U : vUser)
			{
				if (U.UpdateFlag == true)
				{
					U = ReadPasswordAndPermissons();
					U.UserName =UserName;
					break;
				}
			}
			AddUserInformationtoFileByUpdateFlag(vUser);
			cout << "\nUser Update successfully \n";
		}

	}
	else
	{
		cout << "\nUser with [" << UserName << "]Not Found ...\n\n";
	}


}

void findUser()
{
	stUser U;
	vector<stUser>vUser;
	vUser = LoadUsersinVectorFromFile();
	cout << "Please Enter UserName : ";
	
	string UserName;
	getline(cin >> ws, UserName);
	
		if (FindUserUsingUserName(UserName, U))
		{
			cout << "\n\nThe Following are the Client details : \n";
			printUser(U);
		}
		else
		{
			cout << "User with [" << UserName << "Not Found ...\n";
		}
		
}

void NotAccsesPerMissions()
{
	cout << "------------------------------------\n";
	cout << "Access Denied,\nYou don't Have Permissions To Do this\nPlease Concat Your Admin\n";
	cout << "-------------------------------------\n";
}

void stratMainUsersScreen(stUser& User,int choice)
{
	vector<stUser>vUsers;
	vUsers = LoadUsersinVectorFromFile();

	switch (enUserMenuScreen(choice))
	{
	case enUserMenuScreen::ListUser:
		system("cls");
		printUsers();
		system("pause>0");
		system("cls");
		break;

	case enUserMenuScreen::AddNewUser:
		system("cls");
		AddsUsers();
		system("pause>0");
		system("cls");
		break;
	case enUserMenuScreen::DeleteUser:
		system("cls");
		DeleteUserinMenu();
		system("pause>0");
		system("cls");
		break;
	case enUserMenuScreen::UpdateUsers:
		system("cls");
		UpdateUserMenu();
		system("pause>0");
		system("cls");
		break;
	case enUserMenuScreen::FindUser:
		system("cls");
		findUser();
		cout << "\nPress Any Key to Go Back ... " << endl;
		system("pause>0");
		system("cls");
		break;
	case enUserMenuScreen::mainMenuScreen:

		system("cls");
		cout << "Enter Any Key to Back Main Menu..." << endl;
		system("cls");
		break;
	}

}
void PermissionsShowClient(vector<stClient>&VClients)
{
	
	printClients(VClients);
	cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
	system("pause>0");
	system("cls");
	return;
}
void PermissionsAddNewclient(vector<stClient>& VClients)
{
	
	AddClients(VClients);
	cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
	system("pause>0");
	system("cls");
	return;
}
void PermissionsDeleteClient(vector<stClient>& VClients)
{

	DeleteClientByAccountNumber(VClients);
	cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
	system("pause>0");
	system("cls");
	return;
}
void PermissionsUpdateClient(vector<stClient>& VClients)
{	
		updateCLient(VClients);
		cout << "\n\n";
		cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
		system("pause>0");
		system("cls");
		return;
	
}
void PermissionsFindClient(vector<stClient>& VClients)
{
	if (!CheckAccessPermission(enMainMenueOptions::eFindClient))
	{
		NotAccsesPerMissions();
		cout << "\nEnter Any Key to Go Back .. " << endl;
		system("pause>0");
		return;
	}
	stClient Client;
	string AccountNumber = "";
	cout << " please Enter Account Number for Find : ";
	getline(cin >> ws, AccountNumber);
	if (FindClientByAccountNumber(AccountNumber, VClients, Client))
		printClient(Client);
	else
		cout << "Cleint With Account Number(" << AccountNumber << ") Not Found ...";
	cout << "\n\n\nPress Any Key TO Go Back In Main Menu ... " << endl;
	system("pause>0");
	system("cls");
	return;

}

void PermissionsTransAction(vector<stClient>VClients)
{
	system("cls");
	printTransactionsMenu(VClients);
	system("pause");
	system("cls");
	return;
}

void PermissionsManageUaer(stUser &User)
{

	if (!CheckAccessPermission(enMainMenueOptions::eManageUsers))
	{
		NotAccsesPerMissions();
		cout << "\nEnter Any Key to Go Back .. " << endl;
		system("pause>0");
		return;
	}
	else
	{
		int choice1;
		do
		{

			system("cls");
			cout << "======================================================\n";
			cout << "\t\tUser Menu Screen" << endl;
			cout << "======================================================\n";
			cout << left << "[1] List Users .\n";
			cout << left << "[2] Add New User .\n";
			cout << left << "[3] Delete User .\n";
			cout << left << "[4] Update User .\n";
			cout << left << "[5] Find User .\n";
			cout << left << "[6] MainMenu .\n";
			cout << "=======================================================\n";
			choice1 = ReadChoice(1, 6);
			stratMainUsersScreen(User, choice1);
		} while (choice1 != 6 || choice1 > 6);
	}
		return;
}
void startMainMenueScreen(stUser& User, int Choice, vector<stClient>& VClients)
{
	vector<stUser>vUsers;
	vUsers = LoadUsersinVectorFromFile();
	
	stClient Client;
	
	
	switch ((enMainMenuScreen)Choice)
	{

	case enMainMenuScreen::showClientList:
	{
		system("cls");
		PermissionsShowClient(VClients);
		system("cls");
		break;
	}
	case enMainMenuScreen::addNewClient:
	{
		system("cls");
		PermissionsAddNewclient(VClients);
		system("cls");
		break;
	}
	case enMainMenuScreen::deleteClient:
	{
		system("cls");
		PermissionsDeleteClient(VClients);
		system("cls");
		break;
	}
	case enMainMenuScreen::updateClientInfo:
	{
		system("cls");
		PermissionsUpdateClient(VClients);
		system("cls");
		break;
	}

	case enMainMenuScreen::findClient:
	{
		system("cls");
		PermissionsFindClient(VClients);
		system("cls");
		break;
	}

	case enMainMenuScreen::Transctions:
	{
		system("cls");
		PermissionsTransAction(VClients);
		system("cls");
		return;
	}
	case enMainMenuScreen::magageusers:
	{
		system("cls");
		PermissionsManageUaer(User);
		system("cls");
		return;
	}
	case enMainMenuScreen::Logout:
	{
		system("cls");
		cout << "+++++++++++++++++++++++++++++\n";
		cout << "End Login InterFace...\n";
		cout << "++++++++++++++++++++++++++++++\n";
		system("pause>0");
		break;
	}
	}
}

void printMainMenu(stUser &User,vector<stClient>& VClient)
{
	vector<stUser>vUser;
	vUser= LoadUsersinVectorFromFile();
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
		cout << left << "[7] Manage Users .\n";
		cout << left << "[8] Exit .\n";
		cout << "======================================================\n";
		choice = ReadChoice(1,8);
		startMainMenueScreen(User,choice, VClient);
	} while (choice != 8 || choice > 8);
	VClient = SaveClientinVectorFromFile(ClientsFileName);
	vUser = LoadUsersinVectorFromFile();
	
}

bool CheckUserNameAndPasswordifCorretOrNot(string& UserName, string& Password, stUser &User)
{
	vector<stUser>vUsers;
	vUsers = LoadUsersinVectorFromFile();
	for (stUser& U : vUsers)
	{
		if (UserName == U.UserName && Password == U.PassWord)
		{
			User = U;
			return true;
		}
	}
	return false;
}

void login() 
{

	vector<stClient> vClinet = SaveClientinVectorFromFile(ClientsFileName);
	vector<stUser>vUsers=LoadUsersinVectorFromFile();
	

	
	
	string UserName;
	string Password;

	if (vUsers.empty())
	{
		stUser Admin;
		Admin.UserName = "Admin";
		Admin.PassWord = "Admin1234";
		Admin.Permissions = -1;
		AddUserInfotoFile(Admin);
		vUsers = LoadUsersinVectorFromFile();
	}
	
	cout << "=================================================\n";
	cout << "\tUser Login:\n\n";
	cout << "===================================================\n";

		cout << "Enter UserName? ";
		getline(cin >> ws, UserName);
		cout << "Enter Password? ";
		getline(cin >> ws, Password);
		if (CheckUserNameAndPasswordifCorretOrNot(UserName, Password, GeneralUser))
		{
			system("cls");
			printMainMenu(GeneralUser, vClinet);
		}
		else 
		{
			cout << "\nUser with [" << UserName << "] and password [" << Password << "] not found.\n";
			
			cout << "\n\nThat is invalid User , Enter Another User ..  \n";
			system("pause>0");
			system("cls");
			login();
		}	

}

int main()
{
	login();
}
