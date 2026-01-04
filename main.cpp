#include <iostream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


/* ===================== Book ===================== */
class Book
{
private:
    int bookId;
    string title;
    string author;
    string category;
    int availableCopies;

public:
    Book(int id, string t, string a, string c, int copies)
        : bookId(id), title(t), author(a), category(c), availableCopies(copies) {}

    int getBookId() const
    {
        return bookId;
    }

    string getTitle() const
    {
        return title;
    }

    bool isAvailable() const
    {
        return availableCopies > 0;
    }

    void increaseCopies()
    {
        availableCopies++;
    }

    void decreaseCopies()
    {
        if (availableCopies > 0)
            availableCopies--;
    }

    void displayInfo() const
    {
        cout << "ID: " << bookId
             << " | Title: " << title
             << " | Author: " << author
             << " | Category: " << category
             << " | Available: " << availableCopies << endl;
    }
};

/* ===================== Library ===================== */
class Library
{
private:
    vector<Book> books;

public:
    ///1- Add
    void addBook(const Book& book)
    {

    }

    ///2- Update Book
    void updateBook (int bookId, const Book& book)
    {

    }


    ///3- Remove Book
    void removeBook(int bookId)
    {

    }

    ///4- Search Book By ID
    Book* searchBookById(int bookId)
    {

    }

    ///5- Search Book By Title
     Book* searchBookById(string title)
    {

    }


    ///6- Display Books
    void displayBooksWithIDs() const
    {

    }
};


/* ===================== User ===================== */
class User
{
protected:
    int userId;
    string name;
    string password;

public:
    User() : userId(0), name(""), password("") {}
    User(int id, string n, string p): userId(id), name(n), password(p) {}



    virtual void displayMenu() = 0;

    bool login(const string& pass) const
    {
        return password == pass;
    }


    virtual ~User() {}
};


/* ===================== Member ===================== */
class Member : public User
{
private:
    vector<Book*> borrowedBooks;

    // Static container for all members
    static vector<Member> members;   // shared
    static int nextId;               // auto-increment counter

public:
    Member() : User() {}
    Member(int id, string n, string p)
        : User(id, n, p) {}

    /// 1- Register

    void registerMember()
    {
        int id;
        string name, password;

        cout << "\n--- Member Registration ---\n";

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter Password: ";
        getline(cin, password);


        // Assign auto-incremented ID
        userId = nextId++;

        members.emplace_back(id, name, password);
        cout << "Registration successful!\n";
         cout << "Your Member ID is: " << userId << endl;
    }

    ///2- Login

    ///3- BorrowBook Code
    void borrowBook(Library& library, int bookId)
    {

    }

    ///3- Return Book Code
    void returnBook(int bookId)
    {

    }

    ///4- View Books
    void viewBorrowedBooks() const
    {

    }

    ///5- View ALL Books
    void viewAllBooks() const
    {

    }

    ///6- Display Menu
    void displayMenu() override
    {
        int choice;

        do
        {
            cout << "\n--- Member Menu ---\n";
            cout << "1. Borrow Book\n";
            cout << "2. Return Book\n";
            cout << "3. View Borrowed Books\n";
            cout << "4. View All Books\n";
            cout << "0. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                break;
            }
            case 2:
            {

                break;
            }
            case 3:

                break;

            case 4:

                break;

            case 0:
                cout << "Logging out...\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }
        }
        while (choice != 0);
    }

};

vector<Member> Member::members;
int Member::nextId = 1;


/* ===================== Librarian ===================== */
class Librarian : public User
{
public:
    Librarian(int id, string n, string p)
        : User(id, n, p) {}


    ///1- Authentication
    bool authenticateLibrarian()
    {
        string name, password;

        cout << "Enter Librarian Name: ";
        cin >> name;

        cout << "Enter Password: ";
        cin >> password;

        if (this->name == name && this->password == password)
        {
            cout << "Librarian authenticated successfully.\n";
            return true;
        }
        else
        {
            cout << "Invalid name or password.\n";
            return false;
        }
    }

    ///2- Add Book
    void addBook(Library& library)
    {
    }

    ///3- Update Book
    void updateBook (int bookId, const Book& book)
    {

    }

    ///4- Remove Book
    void removeBook(Library& library)
    {
    }

    ///5- Display Borrowed Books
    void displayBorrowedBooks()
    {

    }

    ///6- Search Book By ID
    Book* searchBookById(int bookId)
    {

    }

    ///7- Search Book By Title
     Book* searchBookById(string title)
    {

    }

    ///8- Librarian Menu
    void displayMenu() override
    {
        int choice;

        do
        {
            cout << "\n--- Librarian Menu ---\n";
            cout << "1. Add Book\n";
            cout << "2. Remove Book\n";
            cout << "3. View All Books\n";
            cout << "0. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                ///Add Book
                cout << "Book added successfully.\n";
                break;
            }
            case 2:
            {
                ///Remove Book
                cout << "Book removed successfully.\n";
                break;
            }
            case 3:
                ///Display
                cout << "Book displayed successfully.\n";
                break;
            }
        }
        while (choice != 0);
    }


};






int main()
{
    Librarian librarian(1, "admin", "admin123");

    int choice;
    while (true)
    {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Librarian\n";
        cout << "2. Member\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {

            if (librarian.authenticateLibrarian())
            {
                librarian.displayMenu();
            }
        }
        else if (choice == 2)
        {
            Member currentMember;
            int memberChoice;
            cout << "\n1. Register\n2. Login\nChoice: ";
            cin >> memberChoice;

            if (memberChoice == 1)
            {
                currentMember.registerMember();
            }
            else if (memberChoice == 2)
            {
                ///Login Implementation
            }
        }
        else if (choice == 3)
        {
            cout << "Exiting system...\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}



