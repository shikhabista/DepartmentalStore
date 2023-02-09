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
struct ProductInfo {
    long ProductId;
    string Barcode;
    string ArticleNo;
    string ProductName;
    string ProductUnit;
    long ProductCp;
    long ProductSp;
};

class UserNotFoundException {

};

class Authentication {
    string authFileName = "auth.txt";
public:
    bool Initialize() {

        string username;
        string password;
        string answer;
        while (true) {
            int choice;
            bool isSuccess;

            cout << "\n\t1. Login \n";
            cout << "\t2. Register as a new user \n";
            cout << "\t3. Exit \n";
            cout << "\tYour choice:";
            cin >> choice;

            switch (choice) {
                case 1:
                    isSuccess = Login();
                    if (isSuccess) {
                        return true;
                    } else {
                        cout << "Invalid login credentials" << endl;
                    }
                    break;
                case 2:
                    Register();
                    break;
                default:
                    break;
            }
            char shouldContinue;
            cout << "Press Y/y to continue. Any other key to exit: \t";
            cin >> shouldContinue;
            if (shouldContinue != 'Y' && shouldContinue != 'y') {
                return false;
            }
        }
    }

    bool Login() {
        UserInput user, tempUser;
        fstream authFile;
        authFile.open(authFileName, ios::in | ios::binary);
        cout << "\n\t\tUsername:";
        cin >> user.Username;
        cout << "\t\tPassword: \t";
        cin >> user.Password;

        while (authFile.read((char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if (userNameMatched && passwordMatched) {
                cout << "\t\tLogged in successfully\nWelcome!";
                cout << "|";
                return true;
            } else {
                cout << "\n\t\tSorry! Invalid credentials";
                return false;
            }
        }
        return false;
    }

    void Register() {
        // Register User.
        UserInput user;
        fstream authFile; // fstream => file stream
        authFile.open(authFileName, ios::app | ios::binary);

        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};

class Article {

    string ProductFileName = "Products.txt";

    void CreateProduct() {
        ProductInfo info;
        fstream productFile;
        productFile.open(ProductFileName, ios::app | ios::binary);
        cout << "Enter Product Details to add a product\n";
        cout << "Article Id:\n";
        cin >> info.ProductId;
        cout << "Article Barcode (EAN/BC):\n";
        cin >> info.Barcode;
        cout << "Article No:\n";
        cin >> info.ArticleNo;
        cout << "Article Description:\n";
        cin >> info.ProductName;
        cout << "Purchase Rate:\n";
        cin >> info.ProductCp;
        cout << "Sales Rate:\n";
        cin >> info.ProductSp;

        productFile.write((char *) &info, sizeof(ProductInfo));
    }

    void UpdateProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "Enter Product Id:\t";
        cin >> pInfo.ProductId;

        fstream productFile, tempFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.txt", ios::out | ios::binary | ios::trunc);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.ProductId, pInfo.ProductId) == 0)
        }


    }
};

int main() {
    cout << "****************** * Shinoga Departmental Store * ******************\n";
    cout << "\t\t\t-- Birtamode, Jhapa -- \n";
    cout << "\n\t\tWELCOME TO SHINOGA DEPARTMENTAL STORE\n";
//    cout<< "\t\t\tMain Menu\n";

    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if (!loginSuccess) {
        std::exit(0);
    }

}
