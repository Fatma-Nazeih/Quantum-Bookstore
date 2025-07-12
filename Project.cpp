#include <iostream>
#include <vector>
#include <string>
#include <memory>
using namespace std;

class Book {
protected:
    string isbn, title, author;
    int year;
    double price;

public:
    Book(string i, string t, string a, int y, double p)
        : isbn(i), title(t), author(a), year(y), price(p) {}

    virtual ~Book() {}

    string getISBN() const { return isbn; }
    int getYear() const { return year; }
    string getTitle() const { return title; }
    virtual bool isForSale() const = 0;
    virtual bool isAvailable(int quantity) const = 0;
    virtual double buy(int quantity, string email, string address) = 0;
    virtual void print() const = 0;
};

class PaperBook : public Book {
private:
    int stock;

public:
    PaperBook(string i, string t, string a, int y, double p, int s)
        : Book(i, t, a, y, p), stock(s) {}

    bool isForSale() const override { return true; }

    bool isAvailable(int quantity) const override {
        return stock >= quantity;
    }

    double buy(int quantity, string email, string address) override {
        if (stock < quantity) {
            throw runtime_error("Not enough stock.");
        }
        stock -= quantity;
        cout << "Quantum book store: Shipping paper book to " << address << endl;
        return price * quantity;
    }

    void print() const override {
        cout << "Quantum book store: [PaperBook] Title: " << title << ", Author: " << author
             << ", Year: " << year << ", ISBN: " << isbn << ", Price: " << price
             << ", Stock: " << stock << endl;
    }
};

class EBook : public Book {
private:
    string filetype;

public:
    EBook(string i, string t, string a, int y, double p, string f)
        : Book(i, t, a, y, p), filetype(f) {}

    bool isForSale() const override { return true; }

    bool isAvailable(int quantity) const override {
        return true;
    }

    double buy(int quantity, string email, string address) override {
        cout << "Quantum book store: Sending ebook to email " << email << endl;
        return price * quantity;
    }

    void print() const override {
        cout << "Quantum book store: [EBook] Title: " << title << ", Author: " << author
             << ", Year: " << year << ", ISBN: " << isbn << ", Price: " << price
             << ", Filetype: " << filetype << endl;
    }
};

class ShowcaseBook : public Book {
public:
    ShowcaseBook(string i, string t, string a, int y)
        : Book(i, t, a, y, 0.0) {}

    bool isForSale() const override { return false; }

    bool isAvailable(int quantity) const override {
        return false;
    }

    double buy(int quantity, string email, string address) override {
        throw runtime_error("This book is not for sale.");
    }

    void print() const override {
        cout << "Quantum book store: [ShowcaseBook] Title: " << title << ", Author: " << author
             << ", Year: " << year << ", ISBN: " << isbn << " [Not For Sale]" << endl;
    }
};

class Bookstore {
private:
    vector<shared_ptr<Book>> inventory;

public:
    void addBook(shared_ptr<Book> book) {
        inventory.push_back(book);
    }

    void removeOutdatedBooks(int currentYear, int maxYearsOld) {
        for (auto it = inventory.begin(); it != inventory.end();) {
            if (currentYear - (*it)->getYear() > maxYearsOld) {
                cout << "Quantum book store: Removing outdated book \"" << (*it)->getTitle() << "\"" << endl;
                it = inventory.erase(it);
            } else {
                ++it;
            }
        }
    }

    double buyBook(string isbn, int quantity, string email, string address) {
        for (auto& book : inventory) {
            if (book->getISBN() == isbn) {
                if (!book->isForSale()) {
                    throw runtime_error("Book is not for sale.");
                }
                if (!book->isAvailable(quantity)) {
                    throw runtime_error("Book is not available in required quantity.");
                }
                double amount = book->buy(quantity, email, address);
                cout << "Quantum book store: Purchase complete. Amount paid: " << amount << endl;
                return amount;
            }
        }
        throw runtime_error("Book not found.");
    }

    void showAllBooks() const {
        for (const auto& book : inventory) {
            book->print();
        }
    }
};

class QuantumBookstoreFullTest {
public:
    static void run() {
        Bookstore store;

        store.addBook(make_shared<PaperBook>("ISBN001", "C++ Basics", "Bjarne", 2015, 100.0, 10));
        store.addBook(make_shared<EBook>("ISBN002", "Learn Python", "Guido", 2020, 50.0, "PDF"));
        store.addBook(make_shared<ShowcaseBook>("ISBN003", "Rare Book", "Unknown", 1990));

        cout << "\nQuantum book store: Inventory:\n";
        store.showAllBooks();

        try {
            cout << "\nQuantum book store: Buying paper book:\n";
            store.buyBook("ISBN001", 2, "fatma@example.com", "Giza");

            cout << "\nQuantum book store: Buying ebook:\n";
            store.buyBook("ISBN002", 1, "fatma@example.com", "");

            cout << "\nQuantum book store: Attempting to buy showcase book:\n";
            store.buyBook("ISBN003", 1, "fatma@example.com", "");

        } catch (exception& e) {
            cout << "Quantum book store: Error: " << e.what() << endl;
        }

        cout << "\nQuantum book store: Removing books older than 10 years:\n";
        store.removeOutdatedBooks(2025, 10);

        cout << "\nQuantum book store: Updated Inventory:\n";
        store.showAllBooks();
    }
};

int main() {
    QuantumBookstoreFullTest::run();
    return 0;
}
