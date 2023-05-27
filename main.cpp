#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <conio.h>

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
    long Quantity;
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
            cout << "\t\tPress Y/y to continue. Any other key to exit: ";
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
        cout << "\t\t------------------------------------------------\n\n";
        cout << "\t\tUsername:";
        cin >> user.Username;
        cout << "\t\tPassword:";
        cin >> user.Password;

        while (authFile.read((char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if (userNameMatched && passwordMatched) {
                cout << "\n\t\t****************** Logged in successfully ************\n";
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

        cout << "\t\tUsername:";
        cin >> user.Username;
        cout << "\t\tPassword:";
        cin >> user.Password;
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};

class Article {

    string ProductFileName = "Products.dat";

protected:

    void DisplayProducts() {

        cout << "\t_________________________________________________________________________________________________\n";
        cout << "\tS.N\t| Article No\t| Article Name\t\t| Unit\t\t| CP\t| SP\t| Quantity\t|\n";
        cout << "\t_________________________________________________________________________________________________\n";
        fstream file;
        ProductInfo productInfo{};
        file.open(ProductFileName, ios::in | ios::binary);
        int count = 0;
        while (file.read((char *) &productInfo, sizeof(ProductInfo))) {
            cout << "\t" << ++count << "\t| " << productInfo.ArticleNo << "\t\t| " << productInfo.ArticleName
                 << "\t\t\t| "
                 << productInfo.ProductUnit << "\t\t| " << productInfo.ProductCp << "\t| " << productInfo.ProductSp
                 << "\t| " << productInfo.Quantity << "\t\t|";
            cout << endl;
        }

    }

    void CreateProduct() {
        ProductInfo info, pInfo;
        fstream productFile;
        productFile.open(ProductFileName, ios::app | ios::binary);
        cout << "\t\tEnter Product Details to add a product\n";
        cout << "\t\tArticle Id:";
        cin >> info.ProductId;
        cout << endl;
        cout << "\t\tArticle Barcode (EAN/BC):";
        cin >> info.Barcode;
        cout << "\t\tArticle No:";
        cin >> info.ArticleNo;
        cout << "\t\tArticle Description:";
        cin >> info.ArticleName;
        cout << "\t\tUnit:";
        cin >> info.ProductUnit;
        cout << "\t\tPurchase Rate:";
        cin >> info.ProductCp;
        cout << "\t\tSales Rate:";
        cin >> info.ProductSp;
        cout << "\t\tOpening Quantity:";
        cin >> info.Quantity;

        productFile.write((char *) &info, sizeof(ProductInfo));
    }

    void UpdateProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "\t\tEnter Article no of the article you would like to update:\t";
        cin >> pInfo.ArticleNo;

        fstream productFile, tempFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.ArticleNo, pInfo.ArticleNo) == 0) {
                found = true;
                cout << "\t\tArticle Information:\n";
                cout << "\t\tArticle Id:" << fileData.ProductId << "\t";
                cout << "\t\tArticle Barcode (EAN/BC):" << fileData.Barcode << "\t";
                cout << "\t\tArticle No:" << fileData.ArticleNo << "\t\n";
                cout << "\t\tArticle Description:" << fileData.ArticleName << "\t";
                cout << "\t\tUnit:" << fileData.ProductUnit << "\t";
                cout << "\t\tPurchase Rate:" << fileData.ProductCp << "\t";
                cout << "\t\tSales Rate:" << fileData.ProductSp << "\t\n";
                cout << "\t\tEnter Product Details to update the product\n";
                cout << "\t\tBarcode:";
                cin >> pInfo.Barcode;
                cout << "\t\tArticle No:";
                cin >> pInfo.ArticleNo;
                cout << "\t\tArticle Description:";
                cin >> pInfo.ArticleName;
                cout << "\t\tUnit:";
                cin >> pInfo.ProductUnit;
                cout << "\t\tPurchase Rate:";
                cin >> pInfo.ProductCp;
                cout << "\t\tSales Rate:";
                cin >> pInfo.ProductSp;
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
            cout << "\tProduct details updated successfully" << endl;
        } else {
            cout << "\tNo matching Product found" << endl;
        }
    }

    void FindProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "\t\tEnter the barcode of the article to filter:\t";
        cin >> pInfo.Barcode;

        fstream productFile, tempFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.Barcode, pInfo.Barcode) == 0) {
                found = true;
                cout << "\t____________________________________________________________________________________________________\n";
                cout << "\t| Barcode\t| Article No\t| Article Name\t\t| Unit\t\t| CP\t| SP\t| Quantity\t|\n";
                cout << "\t____________________________________________________________________________________________________\n";
                cout << "\t| " << fileData.Barcode << "\t\t| " << fileData.ArticleNo << "\t\t| " << fileData.ArticleName
                     << "\t\t\t| "
                     << fileData.ProductUnit << "\t\t| " << fileData.ProductCp << "\t| " << fileData.ProductSp
                     << "\t| " << fileData.Quantity << "\t\t|";
                cout << endl;
                tempFile.write((char *) &pInfo, sizeof(ProductInfo));
            } else {
                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            }
        }

//        productFile.close();
//        tempFile.close();
//        ::remove(ProductFileName.c_str());
//        ::rename("temp.dat", ProductFileName.c_str());
        if (!found) {
            cout << "\tNo matching Product found" << endl;
        }
    }

    void DeleteProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "\tEnter the Article no of the article you would like to delete:\t";
        cin >> pInfo.ArticleNo;

        fstream authFile, tempFile;

        authFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (authFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.ArticleNo, pInfo.ArticleNo) != 0) {
                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            } else {
                if (fileData.Quantity > 0) {
                    cout << "\tThe product you are trying to delete has quantity more than 0";
                }
                else {
                    found = true;
                }
            }
        }
        authFile.close();
        tempFile.close();
        ::remove(ProductFileName.c_str());
        ::rename("temp.dat", ProductFileName.c_str());
        if (found) {
            cout << "\tProduct deleted successfully." << endl;
        } else {
            cout << "\tNo matching product found";
        }


    }

public:
    void Initialize() {


        while (true) {
            char shouldContinue;
            char choice;
            cout << "\t\t1. Create Product" << endl;
            cout << "\t\t2. View Product Report" << endl;
            cout << "\t\t3. Update Product" << endl;
            cout << "\t\t4. Delete Product" << endl;
            cout << "\t\t5. Find Product" << endl;

            cout << "\t\tYour choice:";
            cin >> choice;
            cout << endl;
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
                case '5': {
                    FindProduct();
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
