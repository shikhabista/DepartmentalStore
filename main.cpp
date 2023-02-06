#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

struct UserInput {
    char Username[500];
    char Password[500];
};

class UserNotFoundException {

};

class Authentication {
    string authFileName = "auth.dat";
public:
    bool Initialize() {

        string username;
        string password;
        string choice;
        string answer;
        bool isSuccess;
        cout << "\n\n\t\t\tAlready an user?\n\t\tOnly (Y)es or (N)o are acceptable:";
        cin >> answer;
        if (answer == "y" || answer == "Y") {
            cout << "\n\t\tWould you like to login?:";
            cin >> choice;
            if (choice == "y") {
                return Login();
            } else {
                cout<<"Only (Y)es or (N)o are acceptable";
            }
        }
        if (answer == "n" || answer == "N") {
            cout << "Would you like to register a new user?:";
            cin >> choice;
            if (choice == "y") {
                Register();
            } else {
                return false;
            }
        }
        return false;
    }

    bool Login() {
        UserInput user, tempUser;
        fstream authFile;
        authFile.open(authFileName, ios::in | ios::binary);
        cout << "\t\t Username:";
        cin >> user.Username;
        cout << "\t\t  Password: \t";
        cin >> user.Password;

        while (authFile.read((char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if (userNameMatched && passwordMatched) {
                cout<<"Welcome!";
                return true;
            }
        }
        return false;
    }

    void Register() {
        // Register User.
        UserInput user;
        fstream authFile; // fstream => file stream
        // open ( fileName
        authFile.open(authFileName, ios::app | ios::binary);

        cout << "\t\tUsername: \t";
        cin >> user.Username;
        cout << "\t\tPassword: \t";
        cin >> user.Password;
        // write to file
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};




int main() {
    cout<<"****************** * Shinoga Departmental Store * ******************\n";
    cout<< "\t\t\t-- Birtamode, Jhapa -- \n";
    cout<<"\n\t\tWELCOME TO SHINOGA DEPARTMENTAL STORE";
//    cout<< "\t\t\tMain Menu\n";

    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if (!loginSuccess) {
        std::exit(0);
    }

}
