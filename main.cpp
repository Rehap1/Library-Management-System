#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

const int MAX_BORROW = 3;
const int MAX_BORROW_DAYS = 30;

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
                cout << "Book updated successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    /// 3- Remove Book
    void removeBookById(int bookId)
    {
        for (auto it = books.begin(); it != books.end(); ++it)
        {
            if (it->getBookId() == bookId)
            {
                books.erase(it);
                cout << "Book removed successfully.\n";
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
    Book *searchBookByTitle(string title)
    {
        for (auto &book : books)
        {
            if (book.getTitle() == title)
                return &book;
        }
        return nullptr;
    }

    /// 6- Display Books
    void displayBooksWithIDs() const
    {
        if (books.empty())
        {
            cout << "Library is empty.\n";
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

    // Static container for all members
    static vector<Member> members; // shared
    static int nextId;             // auto-increment counter

public:
    Member() : User() {}
    Member(int id, string n, string p)
        : User(id, n, p) {}

    /// 1- Register
    void registerMember(Library &library)
    {
        string name, password;

        cout << "\n--- Member Registration ---\n";

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter Password: ";
        getline(cin, password);

        userId = nextId++;
        this->name = name;
        this->password = password;

        library.addMember(this);
        cout << "Registration successful!\n";
        cout << "Your Member ID is: " << userId << endl;
    }

    /// 2- Login
    static Member *loginMember(Library &library)
    {
        string name;
        string password;

        cout << "\n--- Member Login ---\n";
        cout << "Enter Member Name: ";
        cin >> name;

        cout << "Enter Password: ";
        cin >> password;

        for (Member *member : library.getMembers())
        {
            if (member->name == name && member->password == password)
            {
                cout << "Login successful! Welcome, " << member->name << ".\n";
                return member;
            }
        }

        cout << "Invalid Member ID or Password.\n";
        return nullptr;
    }

    /// 3- BorrowBook Code
    void borrowBook(Library &library, int bookId)
    {
        if (borrowedBooks.size() >= MAX_BORROW)
        {
            cout << "Borrowing limit reached.\n";
            return;
        }

        time_t now = time(nullptr);

        // Check overdue books
        for (const auto &item : borrowedBooks)
        {
            int days = difftime(now, item.second) / (60 * 60 * 24);
            if (days > MAX_BORROW_DAYS)
            {
                cout << "You have overdue books.\n";
                return;
            }
        }

        Book *book = library.searchBookById(bookId);
        if (!book)
        {
            cout << "Book not found.\n";
            return;
        }

        if (!library.borrowBook(bookId))
        {
            cout << "Book not available.\n";
            return;
        }

        borrowedBooks.push_back({book, now});
        cout << "Book borrowed successfully.\n";
    }

    /// 3- Return Book Code
    void returnBook(Library &library, int bookId)
    {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it)
        {
            if (it->first->getBookId() == bookId)
            {
                library.returnBook(bookId);
                borrowedBooks.erase(it);
                cout << "Book returned successfully.\n";
                return;
            }
        }
        cout << "You did not borrow this book.\n";
    }

    /// 4- View Books
    void viewBorrowedBooks() const
    {
        if (borrowedBooks.empty())
        {
            cout << "No borrowed books.\n";
            return;
        }
        time_t now = time(nullptr);

        for (const auto &item : borrowedBooks)
        {
            int days = difftime(now, item.second) / (60 * 60 * 24);
            item.first->displayInfo();
            cout << "Borrowed for: " << days << " days\n";
            cout << "-------------------------\n";
        }
    }

    /// 5- View ALL Books
    void viewAllBooks(const Library &library) const
    {
        library.displayBooksWithIDs();
    }

    /// 6- Display Menu
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
            cout << "0. Logout\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                int bookId;
                cout << "Enter Book ID: ";
                cin >> bookId;
                borrowBook(library, bookId);
                break;
            }

            case 2:
            {
                int bookId;
                cout << "Enter Book ID: ";
                cin >> bookId;
                returnBook(library, bookId);
                break;
            }

            case 3:
                viewBorrowedBooks();
                break;

            case 4:
                viewAllBooks(library);
                break;

            case 0:
                cout << "Logging out...\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 0);
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

    /// 2- Add Book
    void addBook(Library &library)
    {
        int copies;
        string title, author, category;

        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Category: ";
        getline(cin, category);

        cout << "Enter Available Copies: ";
        cin >> copies;

        library.addBook(title, author, category, copies);
    }

    /// 3- Update Book
    void updateBook(Library &library)
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

    /// 4- Remove Book
    void removeBook(Library &library)
    {
        int id;
        cout << "Enter Book ID to remove: ";
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
            cout << "5. Display Borrowed Books Report\n";
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
            case 5:
                displayBorrowedBooks(library);
                break;
            case 0:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

int main()
{
    Library library;
    Librarian librarian(1, "admin", "admin123");

    library.addBook("C++ Programming", "Bjarne Stroustrup", "Programming", 3);
    library.addBook("Algorithms", "Robert Sedgewick", "Computer Science", 1);

    int choice;
    while (true)
    {
        cout << CYAN << BOLD;
        cout << "=========================================\n";
        cout << "     LIBRARY MANAGEMENT SYSTEM\n";
        cout << "=========================================\n";
        cout << RESET;

        cout << GREEN << BOLD << "\n===== MAIN MENU =====\n"<< RESET;
        cout << GREEN << "1. Librarian\n";
        cout << GREEN << "2. Member\n";
        cout << RED << "3. Exit\n"<< RESET;
        cout << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;

        if (choice == 1)
        {

            if (librarian.authenticateLibrarian())
            {
                librarian.displayMenu(library);
            }
        }
        else if (choice == 2)
        {
            Member *currentMember = nullptr;
            int memberChoice;
            cout << "\n1. Register\n2. Login\nChoice: ";
            cin >> memberChoice;

            if (memberChoice == 1)
            {
                currentMember = new Member();
                currentMember->registerMember(library);
            }
            else if (memberChoice == 2)
            {
                currentMember = Member::loginMember(library);
            }
            if (currentMember != nullptr)
            {
                currentMember->displayMenu(library);
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
