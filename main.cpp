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
    char Barcode[500];
    char ArticleNo[500];
    char ArticleName[500];
    char ProductUnit[500];
    long ProductCp;
    long ProductSp;
};

class UserNotFoundException {

};

class Authentication {
    string authFileName = "auth.dat";
public:
    bool Initialize() {

        string username;
        string password;
        string answer;
        while (true) {
            int choice;
            bool isSuccess;

            cout << "\n\t\t1. Login \n";
            cout << "\t\t2. Register as a new user \n";
            cout << "\t\t3. Exit \n";
            cout << "\t\tYour choice:";
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
            cout << "\tPress Y/y to continue. Any other key to exit: ";
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
        cout << "\t\t------------------------------------------------\n";
        cout << "\n\t\tUsername:";
        cin >> user.Username;
        cout << "\t\tPassword:";
        cin >> user.Password;

        while (authFile.read((char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if (userNameMatched && passwordMatched) {
                cout << "\n\t\t************ Logged in successfully ************\n";
                cout << "\n\t\t Welcome! What would you like to do?\n";
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

        cout << "\tUsername:";
        cin >> user.Username;
        cout << "\tPassword:";
        cin >> user.Password;
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};

class Article {

    string ProductFileName = "Products.dat";

protected:

    void DisplayProducts() {
        cout << "S.N \t Article No \t Article Name \n";

        fstream file;
        ProductInfo productInfo{};
        file.open(ProductFileName, ios::in | ios::binary);
        int count = 0;
        while (file.read((char *) &productInfo, sizeof(ProductInfo))) {
            cout << ++count << "\t" << productInfo.ArticleNo;
            cout << endl;
        }

    }

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
        cin >> info.ArticleName;
        cout << "Unit:\n";
        cin >> info.ProductUnit;
        cout << "Purchase Rate:\n";
        cin >> info.ProductCp;
        cout << "Sales Rate:\n";
        cin >> info.ProductSp;

        productFile.write((char *) &info, sizeof(ProductInfo));
    }

    void UpdateProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "Enter Article no of the article you would like to update:\t";
        cin >> pInfo.ArticleNo;

        fstream productFile, tempFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.ArticleNo, pInfo.ArticleNo) == 0) {
                found = true;
                tempFile.write((char *) &pInfo, sizeof(ProductInfo));
            } else {
                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            }
        }

        productFile.close();
        tempFile.close();
        ::remove(ProductFileName.c_str());
        ::rename("temp.dat", ProductFileName.c_str());
        if (found) {
            cout << "Product details updated successfully" << endl;
        } else {
            cout << "No matching Product found" << endl;
        }
    }

    void DeleteProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "Enter the Article no of the article you would like to delete:\t";
        cin >> pInfo.ArticleNo;

        fstream authFile, tempFile;

        authFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (authFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.ArticleNo, pInfo.ArticleNo) != 0) {
                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            } else {
                found = true;
            }
        }
        authFile.close();
        tempFile.close();
        ::remove(ProductFileName.c_str());
        ::rename("temp.dat", ProductFileName.c_str());
        if (found) {
            cout << "Product deleted successfully." << endl;
        } else {
            cout << "No matching product found";
        }


    }

public:
    void Initialize() {


        while (true) {
            char shouldContinue;
            char choice;
            cout << "\t1. Create Product" << endl;
            cout << "\t2. View Product Report" << endl;
            cout << "\t3. Update Product" << endl;
            cout << "\t4. Delete Product" << endl;

            cin >> choice;

            switch (choice) {
                case '1': {
                    CreateProduct();
                    break;
                }
                case '2': {
                    DisplayProducts();
                    break;
                }
                case '3': {
                    UpdateProduct();
                    break;
                }
                case '4': {
                    DeleteProduct();
                    break;
                }
                default:
                    break;
            }
            cout << endl << "Press Y/y to continue. Any other key to exit: \t";
            cin >> shouldContinue;
            if (shouldContinue != 'Y' && shouldContinue != 'y') {
                break;
            }
        }
    }
};

int main() {
    cout << "****************** * Shinoga Departmental Store * ******************\n";
    cout << "\t\t\t\t-- Birtamode, Jhapa -- \n";
    cout << "\n\t\tWELCOME TO SHINOGA DEPARTMENTAL STORE\n";
//    cout<< "\t\t\tMain Menu\n";

    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if (!loginSuccess) {
        std::exit(0);
    }
    Article app;
    app.Initialize();
    return 0;
}
