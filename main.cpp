#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"

const int MAX_BORROW = 3;
const int MAX_BORROW_DAYS = 30;

using namespace std;

// Helpper Functions
bool isValidPositiveInt()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }
    return true;
}

string toLower(string s)
{
    for (char &c : s)
        c = tolower(c);
    return s;
}


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

        cout << left
             << setw(10) << "ID: " << bookId
             << "| " << setw(30) << ("Title: " + title)
             << "| " << setw(28) << ("Author: " + author)
             << "| " << setw(28) << ("Category: " + category)
             << "| " << setw(15) << "Available: " << availableCopies
             << endl;
    }
};


class Member;
/* ===================== Library ===================== */
class Library
{
private:
    vector<Book> books;
    vector<Member *> members;
    int bookCount = 1;

public:
    void addMember(Member *m)
    {
        members.push_back(m);
    }

    const vector<Member *> &getMembers() const
    {
        return members;
    }

    /// 1- Add Book
    void addBook(string title, string author, string category, int copies)
    {
        Book book(bookCount, title, author, category, copies);
        books.push_back(book);
        bookCount++;
    }

    /// 2- Update Book ByID
    void updateBook(int bookId, const Book &book)
    {
        for (auto &b : books)
        {
            if (b.getBookId() == bookId)
            {
                b = book; // replace old book
                cout << GREEN << "\nBook updated successfully.\n"
                     << RESET;
                return;
            }
        }
        cout << RED << "\nBook not found.\n\n"
             << RESET;
    }

    /// 3- Remove Book
    void removeBookById(int bookId)
    {
        for (auto it = books.begin(); it != books.end(); ++it)
        {
            if (it->getBookId() == bookId)
            {
                books.erase(it);
                cout << GREEN << "Book removed successfully.\n"<< RESET;
                return;
            }
        }
    }

    /// 4- Search Book By ID
    Book *searchBookById(int bookId)
    {
        for (auto &book : books)
        {
            if (book.getBookId() == bookId)
                return &book;
        }
        return nullptr;
    }

    /// 5- Search Book By Title
    Book* searchBookByTitle(string title)
    {
        title = toLower(title);
        for (auto &book : books)
        {
            if (toLower(book.getTitle()) == title)
                return &book;
        }
        return nullptr;
    }

    /// 6- Display Books
    void displayALLBooks() const
    {
        if (books.empty())
        {
            cout << RED << "\nLibrary is empty!.\n"
                 << RESET;
            return;
        }

        for (const auto &book : books)
        {

            book.displayInfo();
        }
    }

    /// 7- Borrow Book
    bool borrowBook(int bookId)
    {
        Book *book = searchBookById(bookId);
        if (!book || !book->isAvailable())
            return false;

        book->decreaseCopies();
        return true;
    }

    /// 8- Return Book
    bool returnBook(int bookId)
    {
        Book *book = searchBookById(bookId);
        if (!book)
            return false;

        book->increaseCopies();
        return true;
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
    User(int id, string n, string p) : userId(id), name(n), password(p) {}

    virtual void displayMenu(Library &library) = 0;

    bool login(const string &pass) const
    {
        return password == pass;
    }

    virtual ~User() {}
};

/* ===================== Member ===================== */
class Member : public User
{
private:
    vector<pair<Book *, time_t>> borrowedBooks;
    static int nextId;             // auto-increment counter

public:
    Member() : User() {}
    Member(int id, string n, string p)
        : User(id, n, p) {}

    /// 1- Register
    void registerMember(Library &library)
    {
        string name, password;

        cout << "\n--- Member Registration ---\n\n";

        cout << YELLOW << "Enter Name: " << RESET;
        cin.ignore();
        getline(cin, name);

        cout << YELLOW << "Enter Password: " << RESET;
        getline(cin, password);

        userId = nextId++;
        this->name = name;
        this->password = password;

        library.addMember(this);
        cout << GREEN << "\nRegistration successful!\n"
             << RESET;
        cout << GREEN << "Your Member ID is: " << userId << RESET << endl;
    }

    /// 2- Login
    static Member *loginMember(Library &library)
    {
        string name;
        string password;

        cout << "\n--- Member Login ---\n";
        cout << YELLOW << "Enter Member Name: " << RESET;
        cin >> name;

        cout << YELLOW << "Enter Password: " << RESET;
        cin >> password;

        for (Member *member : library.getMembers())
        {
            if (member->name == name && member->password == password)
            {
                cout << GREEN << "Login successful! Welcome, " << member->name << ".\n"
                     << RESET;
                return member;
            }
        }

        cout << RED << "Invalid Member ID or Password.\n"
             << RESET;
        return nullptr;
    }

    /// 3- BorrowBook Code
    void borrowBook(Library &library, int bookId)
    {
        if (borrowedBooks.size() >= MAX_BORROW)
        {
            cout << RED << "Borrowing limit reached.\n"<< RESET;
            return;
        }

        time_t now = time(nullptr);

        // Check overdue books
        for (const auto &item : borrowedBooks)
        {
            int days = difftime(now, item.second) / (60 * 60 * 24);
            if (days > MAX_BORROW_DAYS)
            {
                cout << RED << "You have overdue books, Must Return Book First\n"<< RESET;
                return;
            }
        }

        Book *book = library.searchBookById(bookId);
        if (!book)
        {
            cout << RED << "Book not found.\n"
                 << RESET;
            return;
        }

        if (!library.borrowBook(bookId))
        {
            cout << RED << "Book not available.\n"<< RESET;
            return;
        }

        borrowedBooks.push_back({book, now});
        cout << GREEN << "Book borrowed successfully.\n"
             << RESET;
    }

    /// 4- Return Book Code
    void returnBook(Library &library, int bookId)
    {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it)
        {
            if (it->first->getBookId() == bookId)
            {
                library.returnBook(bookId);
                borrowedBooks.erase(it);
                cout << GREEN << "Book returned successfully.\n"
                     << RESET;
                return;
            }
        }
        cout << RED << "You did not borrow this book.\n"
             << RESET;
    }

    /// 5- View Borrowed Books
    void viewBorrowedBooks() const
    {
        if (borrowedBooks.empty())
        {
            cout << RED << "No borrowed books.\n"
                 << RESET;
            return;
        }
        time_t now = time(nullptr);

        for (const auto &item : borrowedBooks)
        {
            int days = difftime(now, item.second) / (60 * 60 * 24);
            item.first->displayInfo();
            cout << YELLOW << "Borrowed for: " << days << " days\n"
                 << RESET;
            cout << "-------------------------\n";
        }
    }

    /// 5- View ALL Books
    void viewAllBooks(const Library &library) const
    {
        library.displayALLBooks();
    }


    ///6- Search Book By Title
    void searchBookByTitleUser(Library &library)
    {
        string title;
        cout << YELLOW << "Enter Book Title: " << RESET;
        cin.ignore();
        getline(cin, title);

        if (title.empty())
        {
            cout << RED << "Title cannot be empty.\n" << RESET;
            return;
        }

        Book *book = library.searchBookByTitle(title);
        if (!book)
        {
            cout << RED << "No book exists with this title.\n" << RESET;
            return;
        }

        cout << GREEN << "\nBook Found:\n" << RESET;
        book->displayInfo();
    }

    /// 8- Display Menu
    void displayMenu(Library &library) override
    {
        int choice;

        do
        {
            cout << "\n--- Member Menu ---\n";
            cout << "1. Borrow Book\n";
            cout << "2. Return Book\n";
            cout << "3. View Borrowed Books\n";
            cout << "4. View All Books\n";
            cout << "5. Search Book By Title\n";
            cout << "0. Logout\n";
            cout << YELLOW << "Choice: " << RESET;
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                int bookId;
                cout << YELLOW << "Enter Book ID: " << RESET;
                cin >> bookId;

                if (!isValidPositiveInt() || bookId <= 0)
                {
                    cout << RED << "Invalid Book ID. Please enter a positive number.\n"
                         << RESET;
                    break;
                }

                borrowBook(library, bookId);
                break;
            }

            case 2:
            {
                int bookId;
                cout << YELLOW << "Enter Book ID: " << RESET;
                cin >> bookId;

                if (!isValidPositiveInt() || bookId <= 0)
                {
                    cout << RED << "Invalid Book ID. Please enter a positive number.\n"
                         << RESET;
                    break;
                }

                returnBook(library, bookId);
                break;
            }

            case 3:
                viewBorrowedBooks();
                break;

            case 4:
                viewAllBooks(library);
                break;

            case 5:
                searchBookByTitleUser(library);
                break;

            case 0:
                cout << "Logging out...\n";
                break;

            default:
                cout << RED << "Invalid choice.\n"
                     << RESET;
            }
        }
        while (choice != 0);
    }

    string getName() const
    {
        return name;
    }
    string getPassword() const
    {
        return password;
    }
};

int Member::nextId = 1;

/* ===================== Librarian ===================== */
class Librarian : public User
{
public:
    Librarian(int id, string n, string p)
        : User(id, n, p) {}

    /// 1- Authentication
    bool authenticateLibrarian()
    {
        string name, password;

        cout << YELLOW << "Enter Librarian Name: " << RESET;
        cin >> name;

        cout << YELLOW << "Enter Password: " << RESET;
        cin >> password;

        if (this->name == name && this->password == password)
        {
            cout << GREEN << "\nLibrarian authenticated successfully.\n"
                 << RESET;
            return true;
        }
        else
        {
            cout << RED << "\nInvalid name or password.\n\n"
                 << RESET;
            return false;
        }
    }

    /// 2- Add Book
    void addBook(Library &library)
    {
        int copies;
        string title, author, category;

        cin.ignore();

        cout << YELLOW << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Category: ";
        getline(cin, category);

        cout << "Enter Available Copies: ";
        cin >> copies;
        cout << RESET;

        library.addBook(title, author, category, copies);
    }

    /// 3- Update Book
    void updateBook(Library &library)
    {
        int id, copies;
        string title, author, category;

        cout << YELLOW << "Enter Book ID to update: ";
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

    /// 4- Remove Book
    void removeBook(Library &library)
    {
        int id;
        cout << YELLOW << "Enter Book ID to remove: ";
        cin >> id;

        library.removeBookById(id);
    }

    /// 5- Display Borrowed Books
    void displayBorrowedBooks(Library &library)
    {
        cout << "\n--- Borrowed Books Report ---\n";

        for (Member *member : library.getMembers())
        {
            cout << "Member: " << member->getName() << endl;
            member->viewBorrowedBooks();
        }
    }

    /// 6- Search Book By ID
    Book *searchBookById(Library &library, int bookId)
    {
        return library.searchBookById(bookId);
    }

    /// 7- Search Book By Title
    Book *searchBookByTitle(Library &library, string title)
    {
        return library.searchBookByTitle(title);
    }

    /// 8- Librarian Menu
    void displayMenu(Library &library) override
    {
        int choice;

        do
        {
            cout << "\n--- Librarian Menu ---\n";
            cout << "1. Add Book\n";
            cout << "2. Update Book\n";
            cout << "3. Remove Book\n";
            cout << "4. Display All Books\n";
            cout << "5. Search Book By ID\n";
            cout << "6. Search Book By Title\n";
            cout << "7. Display Borrowed Books Report\n";
            cout << "0. Logout\n";
            cout << YELLOW << "Choice: " << RESET;
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
                cout << "\n========================================================================================\n\n";
                library.displayALLBooks();
                cout << "\n========================================================================================\n\n";
                break;
            case 5:
            {
                int bookId;
                cout << YELLOW << "Enter Book ID to search: " << RESET;
                cin >> bookId;

                if (!isValidPositiveInt() || bookId <= 0)
                {
                    cout << RED << "Invalid Book ID. Please enter a positive number.\n"
                         << RESET;
                    break;
                }

                Book *book = searchBookById(library, bookId);

                if (book)
                {
                    cout << GREEN << "\nBook found:\n" << RESET;
                    book->displayInfo();
                }
                else
                {
                    cout << RED << "\nNo book exists with this ID.\n"
                         << RESET;
                }
                break;
            }

            case 6:
            {
                string title;
                cout << YELLOW << "Enter Book Title to search: " << RESET;
                cin.ignore();
                getline(cin, title);
                Book *book = searchBookByTitle(library, title);
                if (book)
                {
                    cout << GREEN << "\nBook found:\n"
                         << RESET;
                    book->displayInfo();
                }
                else
                {
                    cout << RED << "\nBook not found.\n"
                         << RESET;
                }
                break;
            }
            case 7:
                displayBorrowedBooks(library);
                break;
            case 0:
                cout << "Logging out...\n";
                break;
            default:
                cout << RED << "Invalid choice.\n"
                     << RESET;
            }
        }
        while (choice != 0);
    }
};

int main()
{
    Library library;
    Librarian librarian(1, "admin", "admin123");

    library.addBook("C++ Programming", "Bjarne Stroustrup", "Programming", 3);
    library.addBook("Algorithms", "Robert Sedgewick", "Computer Science", 1);

    string choice;
    while (true)
    {

        cout << BLUE << BOLD;
        cout << "=========================================\n";
        cout << "     LIBRARY MANAGEMENT SYSTEM\n";
        cout << "=========================================\n";
        cout << RESET;

        cout << BLUE << BOLD << "\n===== MAIN MENU =====\n";
        cout << "1. Librarian\n";
        cout << "2. Member\n";
        cout << RED << "3. Exit\n"
             << RESET;
        cout << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;

        if (choice == "1")
        {

            if (librarian.authenticateLibrarian())
            {
                librarian.displayMenu(library);
            }
        }
        else if (choice == "2")
        {
            Member *currentMember = nullptr;
            string memberChoice;
            cout << "\n1. Register\n2. Login\nChoice: ";
            cin >> memberChoice;

            if (memberChoice == "1")
            {
                currentMember = new Member();
                currentMember->registerMember(library);
            }
            else if (memberChoice == "2")
            {
                currentMember = Member::loginMember(library);
            }
            else
            {
                cout <<RED<< "\nInvalid choice. Try again.\n"<<RESET;
            }

            if (currentMember != nullptr)
            {
                currentMember->displayMenu(library);
            }
        }
        else if (choice == "3")
        {
            cout << "Exiting system...\n";
            break;
        }
        else
        {
            cout <<RED<< "\nInvalid choice. Try again.\n"<<RESET;
        }
    }
    return 0;
}
