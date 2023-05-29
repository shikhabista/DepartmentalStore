#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

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

struct StockMovement {
    long ProductId;
    char ArticleNo[500];
    char ArticleName[500];
    char ProductUnit[500];
    long Quantity;
    long Rate;
    char LogMode[50];
    char Barcode[50];
};

struct StockMovementReturnDto {
    long quantity;
    long rate;
};

struct ArticleStartingNo {
    long Id;
};


void AppHeader() {
    cout << endl;
    cout << "***********************************************************************************************" << endl;
    cout << "******************************    STOCK MANAGEMENT SYSTEM    **********************************" << endl;
    cout << "***********************************************************************************************" << endl;
    cout << endl;
}


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
    string MovementFileName = "stock_movement.dat";
    string ArticleStartingNoFileName = "article_starting_no.dat";

protected:

    void LastPrompt(string message = "") {
        char x[50];
        cout << endl << message;
        cout << endl << "Press any key to continue.";
        cin >> x;
    }

    void DisplayProducts() {

        cout << "\t_________________________________________________________________________________________________________________________\n";
        cout << "\tId\t| Barcode\t\t| Article No\t| Article Name\t\t| Unit\t\t| CP\t| SP\t| Quantity\t|\n";
        cout << "\t_________________________________________________________________________________________________________________________\n";
        fstream file;
        ProductInfo productInfo{};
        file.open(ProductFileName, ios::in | ios::binary);
        int count = 0;
        while (file.read((char *) &productInfo, sizeof(ProductInfo))) {
            cout << "\t" << productInfo.ProductId << "\t| " << productInfo.Barcode << "\t\t| " << productInfo.ArticleNo << "\t\t| "
                 << productInfo.ArticleName
                 << "\t\t\t| "
                 << productInfo.ProductUnit << "\t\t| " << productInfo.ProductCp << "\t| " << productInfo.ProductSp
                 << "\t| " << productInfo.Quantity << "\t\t|";
            cout << endl;
        }
        LastPrompt();
    }

    long GetNextProductId() {
        ArticleStartingNo startingNo;
        fstream articleStartingNoFile;
        articleStartingNoFile.open(ArticleStartingNoFileName, ios::binary | ios::in);
        long nextStartingNo = 1;
        while (articleStartingNoFile.read((char *) &startingNo, sizeof(ArticleStartingNo))) {
            nextStartingNo = startingNo.Id + 1;
            break;
        }
        articleStartingNoFile.close();
        return nextStartingNo;
    }

    void IncrementProductId(long newId) {
        ArticleStartingNo startingNo;
        fstream articleStartingNoFile;
        ::remove(ArticleStartingNoFileName.c_str());
        articleStartingNoFile.open(ArticleStartingNoFileName, ios::binary | ios::out | ios::trunc);
        startingNo.Id = newId;
        articleStartingNoFile.write((char *) &startingNo, sizeof(ArticleStartingNo));
        articleStartingNoFile.close();
    }

    void CreateProduct() {
        ProductInfo info, pInfo;
        fstream productFile;
        fstream articleStartingNo;
        productFile.open(ProductFileName, ios::app | ios::binary);
        info.ProductId = GetNextProductId();
        cout << "\t\tEnter Article Details: \n";
        cout << endl;
//        cout << "\t\tEnter Article Id:";
//        cin >> info.ProductId;
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
        IncrementProductId(info.ProductId);
        LastPrompt("Article Added");
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
                pInfo.ProductId = fileData.ProductId;
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
        LastPrompt("\t\tArticle Updated");

    }

    void FindProduct() {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "\t\tEnter the barcode of the article to filter:\t";
        cin >> pInfo.Barcode;
        fstream productFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.Barcode, pInfo.Barcode) == 0) {
                found = true;
                cout
                        << "\t_________________________________________________________________________________________________________________________\n";
                cout << "\t| Id\t| Barcode\t\t| Article No\t| Article Name\t\t| Unit\t\t| CP\t| SP\t| Quantity\t|\n";
                cout
                        << "\t_________________________________________________________________________________________________________________________\n";
                cout << "\t| " << fileData.ProductId << "\t| " << fileData.Barcode << "\t\t| " << fileData.ArticleNo
                     << "\t\t| " << fileData.ArticleName
                     << "\t\t\t| "
                     << fileData.ProductUnit << "\t\t| " << fileData.ProductCp << "\t| " << fileData.ProductSp
                     << "\t| " << fileData.Quantity << "\t\t|";
                cout << endl;
            }
        }
        if (!found) {
            cout << "\tNo matching Product found" << endl;
        }

        productFile.close();
        LastPrompt();
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
                found = true;
                if (fileData.Quantity > 0) {
                    tempFile.write((char *) &fileData, sizeof(ProductInfo));
                    cout << "\tThe product you are trying to delete has quantity more than 0";
                } else {
                    cout << "\tArticle deleted" <<endl;
                }
            }
        }
        authFile.close();
        tempFile.close();
        ::remove(ProductFileName.c_str());
        ::rename("temp.dat", ProductFileName.c_str());
        if (!found)
        {
            LastPrompt("\tNo matching product found");
        }
        else
        {
            LastPrompt();
        }

    }

    void PerformArticleTransaction_UI(bool isOut) {
        ProductInfo pInfo, fileData;
        bool found = false;
        cout << "\t\tEnter the barcode of the article:\t";
        cin >> pInfo.Barcode;
        fstream productFile;
        fstream tempFile;

        productFile.open(ProductFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary);
        while (productFile.read((char *) &fileData, sizeof(ProductInfo))) {
            if (::strcmp(fileData.Barcode, pInfo.Barcode) == 0) {
                found = true;
                cout
                        << "\t____________________________________________________________________________________________________\n";
                cout << "\t| Barcode\t| Article No\t| Article Name\t\t| Unit\t\t| CP\t| SP\t| Quantity\t|\n";
                cout
                        << "\t____________________________________________________________________________________________________\n";
                cout << "\t| " << fileData.Barcode << "\t\t| " << fileData.ArticleNo << "\t\t| " << fileData.ArticleName
                     << "\t\t\t| "
                     << fileData.ProductUnit << "\t\t| " << fileData.ProductCp << "\t| " << fileData.ProductSp
                     << "\t| " << fileData.Quantity << "\t\t|";
                cout << endl;
                StockMovementReturnDto dto = PerformArticleTransaction(false, fileData);
                if (isOut) {
                    fileData.Quantity -= dto.quantity;
                    fileData.ProductSp = dto.rate;
                } else {
                    fileData.Quantity += dto.quantity;
                    fileData.ProductCp = dto.rate;
                }

                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            } else {
                tempFile.write((char *) &fileData, sizeof(ProductInfo));
            }
        }
        tempFile.close();
        productFile.close();
        ::remove(ProductFileName.c_str());
        ::rename("temp.dat", ProductFileName.c_str());

        if (!found) {
            LastPrompt("Article not found");
        } else {
            LastPrompt("Article Transaction Added");
        }
    }

    StockMovementReturnDto PerformArticleTransaction(bool isOut, ProductInfo article) {
        StockMovementReturnDto dto;
        cout << "\t\t Quantity: \t";
        cin >> dto.quantity;
        cout << "\t\t Rate:\t";
        cin >> dto.rate;
        StockMovement movement;
        fstream movementFile;
        movementFile.open(MovementFileName, ios::binary | ios::app);
        strcpy(movement.ArticleName, article.ArticleName);
        strcpy(movement.ArticleNo, article.ArticleNo);
        strcpy(movement.Barcode, article.Barcode);
        movement.ProductId = article.ProductId;
        if (isOut) {
            ::strcpy(movement.LogMode, "OUT");
        } else {
            ::strcpy(movement.LogMode, "IN");
        }
        movement.Quantity = dto.quantity;
        movement.Rate = dto.rate;
        movementFile.write((char *) &movement, sizeof movement);
        return dto;
    }

public:


    void Initialize() {

        while (true) {
            ::system("cls");
            AppHeader();

            char choice;
            cout << "\t\t1. Create Product" << endl;
            cout << "\t\t2. Find Product" << endl;
            cout << "\t\t3. Update Product" << endl;
            cout << "\t\t4. Delete Product" << endl;
            cout << "\t\t5. View Product Report" << endl;
            cout << "\t\t6. Article IN" << endl;
            cout << "\t\t7. Article OUT" << endl;
            cout << "\t\t8. Exit" << endl;

            cout << "\t\tYour choice:";
            cin >> choice;
            cout << endl;
            switch (choice) {
                case '1': {
                    CreateProduct();
                    break;
                }
                case '2': {
                    FindProduct();
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
                    DisplayProducts();
                    break;
                }
                case '6' : {
                    PerformArticleTransaction_UI(false);
                    break;
                }
                case '7' : {
                    PerformArticleTransaction_UI(true);
                    break;
                }
                default: {
                    cout << "Thank you for using Stock Management Software!!";
                    std::exit(0);
                }
            }
//            cin >> shouldContinue;
//            if (shouldContinue != 'Y' && shouldContinue != 'y') {
//                break;
//            }
        }
    }
};


int main() {
    AppHeader();

    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if (!loginSuccess) {
        std::exit(0);
    }
    Article app;
    app.Initialize();
    return 0;
}
