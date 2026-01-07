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
    ///1- Add Book
    void addBook(const Book& book)
    {
        books.push_back(book);
    }

    ///2- Update Book ByID
    void updateBook (int bookId, const Book& book)
    {
        for (auto& b : books)
        {
            if (b.getBookId() == bookId)
            {
                b = book; // replace old book
                cout << "Book updated successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }


    ///3- Remove Book
    void removeBookById(int bookId)
    {
        for(auto it = books.begin(); it != books.end(); ++it)
        {
            if (it->getBookId() == bookId)
            {
                books.erase(it);
                cout << "Book removed successfully.\n";
                return;
            }
        }
    }

    ///4- Search Book By ID
    Book* searchBookById(int bookId)
    {
        for (auto& book : books)
        {
            if (book.getBookId() == bookId)
                return &book;
        }
        return nullptr;
    }

    ///5- Search Book By Title
    Book* searchBookByTitle(string title)
    {
        for (auto& book : books)
        {
            if (book.getTitle() == title)
                return &book;
        }
        return nullptr;
    }

    ///6- Display Books
    void displayBooksWithIDs() const
    {
         if (books.empty())
        {
            cout << "Library is empty.\n";
            return;
        }

        for (const auto& book : books)
        {
            book.displayInfo();
        }
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
        string name, password;

        cout << "\n--- Member Registration ---\n";

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter Password: ";
        getline(cin, password);

        userId = nextId++;

        //emplace_back to take constructor arguments directly
        members.emplace_back(userId, name, password);
        cout << "Registration successful!\n";
        cout << "Your Member ID is: " << userId << endl;
    }

    ///2- Login
    bool loginMember()
    {
        string name;
        string password;

        cout << "\n--- Member Login ---\n";
        cout << "Enter Member Name: ";
        cin >> name;

        cout << "Enter Password: ";
        cin >> password;

        for (const auto& member : members)
        {
            if (member.name == name && member.password == password)
            {
                cout << "Login successful! Welcome, " << member.name << ".\n";
                // Copy member details to current object
                this->userId = member.userId;
                this->name = member.name;
                this->password = member.password;
                return true;
                
            }
        }

        cout << "Invalid Member ID or Password.\n";
        return false;
    }

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
        int id, copies;
        string title, author, category;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Category: ";
        getline(cin, category);

        cout << "Enter Available Copies: ";
        cin >> copies;

        library.addBook(Book(id, title, author, category, copies));
    }

    ///3- Update Book
    void updateBook (Library& library)
    {
        int id, copies;
        string title, author, category;

        cout << "Enter Book ID to update: ";
        cin >> id;
        cin.ignore();

        cout << "Enter New Title: ";
        getline(cin, title);

        cout << "Enter New Author: ";
        getline(cin, author);

        cout << "Enter New Category: ";
        getline(cin, category);

        cout << "Enter New Available Copies: ";
        cin >> copies;

        library.updateBook(id, Book(id, title, author, category, copies));
    }

    ///4- Remove Book
    void removeBook(Library& library)
    {
        int id;
        cout << "Enter Book ID to remove: ";
        cin >> id;

        library.removeBookById(id);
    }

    ///5- Display Borrowed Books
    void displayBorrowedBooks()
    {
        cout << "Feature not implemented yet.\n";
    }

    ///6- Search Book By ID
    Book* searchBookById(int bookId)
    {

    }

    ///7- Search Book By Title
     Book* searchBookByTitle(string title)
    {

    }

    ///8- Librarian Menu
    void displayMenu() override
    {
        Library library; // Local library instance for demonstration
        int choice;

        do
        {
            cout << "\n--- Librarian Menu ---\n";
            cout << "1. Add Book\n";
            cout << "2. Update Book\n";
            cout << "3. Remove Book\n";
            cout << "4. Display All Books\n";
            cout << "0. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                addBook(library);
                break;
            case 2:
                updateBook(library);
                break;
            case 3:
                removeBook(library);
                break;
            case 4:
                library.displayBooksWithIDs();
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
                bool loginSuccess = currentMember.loginMember();
                if (loginSuccess)
                {
                    currentMember.displayMenu();
                }
                
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



