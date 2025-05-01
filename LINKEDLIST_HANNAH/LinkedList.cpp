#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

//===============================================
// SHARED STRUCTURES AND FUNCTIONS ACROSS QUESTIONS
//===============================================

// Convert date from DD/MM/YYYY to YYYY-MM-DD format
std::string convertDateFormat(const std::string& date) {
    std::stringstream ss(date);
    std::string day, month, year;
    std::getline(ss, day, '/');
    std::getline(ss, month, '/');
    std::getline(ss, year, '/');
    return year + "-" + month + "-" + day;
}

// Function to extract column using delimiter logic
string getColumn(const string& line, int colIndex) {
    size_t start = 0;
    size_t end = line.find(',');
    int currentIndex = 0;

    while (currentIndex < colIndex && end != string::npos) {
        start = end + 1;
        end = line.find(',', start);
        currentIndex++;
    }

    if (end == string::npos) end = line.length();
    return line.substr(start, end - start);
}

// Forward declarations for various run functions
void runQuestion1();
void runQuestion2();
void runQuestion3();

//===============================================
// QUESTION 1: SORTING TRANSACTIONS BY DATE
//===============================================

// Transaction structure for Q1
struct Q1Transaction {
    std::string customerID;
    std::string product;
    std::string category;
    double price;
    std::string date;
    std::string paymentMethod;

    Q1Transaction() : customerID(""), product(""), category(""), price(0.0), date(""), paymentMethod("") {}

    Q1Transaction(std::string cid, std::string prod, std::string cat, double pr, std::string dt, std::string pm)
        : customerID(cid), product(prod), category(cat), price(pr), date(dt), paymentMethod(pm) {}

    friend std::ostream& operator<<(std::ostream& os, const Q1Transaction& t) {
        os << t.customerID << "," << t.product << "," << t.category << ","
           << t.price << "," << t.date << "," << t.paymentMethod;
        return os;
    }
};

// Node structure for the transaction linked list in Q1
struct Q1Node {
    Q1Transaction data;
    Q1Node* next;
    
    Q1Node(const Q1Transaction& t) : data(t), next(nullptr) {}
};

// Linked List class for transactions in Q1
class Q1TransactionList {
private:
    Q1Node* head;
    int size;

public:
    Q1TransactionList() : head(nullptr), size(0) {}

    ~Q1TransactionList() {
        Q1Node* current = head;
        while (current != nullptr) {
            Q1Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void push_back(const Q1Transaction& t) {
        Q1Node* newNode = new Q1Node(t);
        
        if (head == nullptr) {
            head = newNode;
        } else {
            Q1Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    int getSize() const {
        return size;
    }

    Q1Node* getHead() const {
        return head;
    }

    void setHead(Q1Node* newHead) {
        head = newHead;
    }
};

// Node for storing customer IDs in Q1
struct Q1CustomerNode {
    string id;
    Q1CustomerNode* next;
    Q1CustomerNode(string i) : id(i), next(nullptr) {}
};

// Node for date-count pairs in Q1
struct Q1DateCountNode {
    string date;
    int count;
    Q1DateCountNode* next;
    Q1DateCountNode(string d, int c) : date(d), count(c), next(nullptr) {}
};

// Linked list for customer IDs in Q1
class Q1CustomerList {
private:
    Q1CustomerNode* head;
public:
    Q1CustomerList() : head(nullptr) {}

    ~Q1CustomerList() {
        Q1CustomerNode* current = head;
        while (current != nullptr) {
            Q1CustomerNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(const string& id) {
        if (!contains(id)) {
            Q1CustomerNode* newNode = new Q1CustomerNode(id);
            newNode->next = head;
            head = newNode;
        }
    }

    bool contains(const string& id) const {
        Q1CustomerNode* current = head;
        while (current != nullptr) {
            if (current->id == id) return true;
            current = current->next;
        }
        return false;
    }
};

// Linked list for date-count pairs in Q1
class Q1DateCountList {
private:
    Q1DateCountNode* head;
public:
    Q1DateCountList() : head(nullptr) {}

    ~Q1DateCountList() {
        Q1DateCountNode* current = head;
        while (current != nullptr) {
            Q1DateCountNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insertOrIncrement(const string& date) {
        Q1DateCountNode* current = head;
        while (current != nullptr) {
            if (current->date == date) {
                current->count++;
                return;
            }
            current = current->next;
        }
        // Not found, insert at end
        Q1DateCountNode* newNode = new Q1DateCountNode(date, 1);
        if (head == nullptr) {
            head = newNode;
        } else {
            current = head;
            while (current->next != nullptr)
                current = current->next;
            current->next = newNode;
        }
    }

    Q1DateCountNode* getHead() const { return head; }
    void setHead(Q1DateCountNode* newHead) { head = newHead; }
};

// Parse CSV file for transactions in Q1
Q1TransactionList Q1ParseCSV(const std::string& filename) {
    Q1TransactionList transactions;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return transactions;
    }

    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string customerID, product, category, priceStr, date, paymentMethod;

        std::getline(ss, customerID, ',');
        std::getline(ss, product, ',');
        std::getline(ss, category, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, date, ',');
        std::getline(ss, paymentMethod);

        double price = std::stod(priceStr);
        date = convertDateFormat(date);

        transactions.push_back(Q1Transaction(customerID, product, category, price, date, paymentMethod));
    }
    file.close();
    return transactions;
}

// Comparison function for transactions in Q1
bool Q1CompareTransactions(const Q1Transaction& a, const Q1Transaction& b) {
    return a.date < b.date;
}

// Function to split the transaction linked list into two halves in Q1
Q1Node* Q1FindMiddle(Q1Node* head) {
    if (head == nullptr) return nullptr;
    
    Q1Node* slow = head;
    Q1Node* fast = head->next;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// Function to merge two sorted transaction linked lists in Q1
Q1Node* Q1Merge(Q1Node* left, Q1Node* right) {
    // Create a dummy node on the heap to avoid stack issues
    Q1Node* dummy = new Q1Node(Q1Transaction());
    Q1Node* tail = dummy;
    
    while (left != nullptr && right != nullptr) {
        if (Q1CompareTransactions(left->data, right->data)) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    
    // Attach remaining nodes
    if (left != nullptr) {
        tail->next = left;
    } else {
        tail->next = right;
    }
    
    // Save the result
    Q1Node* result = dummy->next;
    
    // Clean up the dummy node
    dummy->next = nullptr;
    delete dummy;
    
    return result;
}

// Merge sort for transaction linked list in Q1
Q1Node* Q1MergeSort(Q1Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // Find the middle of the list
    Q1Node* middle = Q1FindMiddle(head);
    Q1Node* secondHalf = middle->next;
    middle->next = nullptr; // Split the list into two halves
    
    // Recursively sort both halves
    Q1Node* left = Q1MergeSort(head);
    Q1Node* right = Q1MergeSort(secondHalf);
    
    // Merge the sorted halves
    return Q1Merge(left, right);
}

// Find middle node for DateCountNode list in Q1
Q1DateCountNode* Q1FindMiddleDateCount(Q1DateCountNode* head) {
    if (!head) return nullptr;
    Q1DateCountNode* slow = head;
    Q1DateCountNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge two sorted DateCountNode lists in Q1
Q1DateCountNode* Q1MergeDateCount(Q1DateCountNode* left, Q1DateCountNode* right) {
    Q1DateCountNode dummy("", 0);
    Q1DateCountNode* tail = &dummy;

    while (left && right) {
        if (left->date <= right->date) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = (left) ? left : right;
    return dummy.next;
}

// Merge sort for DateCountNode linked list in Q1
Q1DateCountNode* Q1MergeSortDateCount(Q1DateCountNode* head) {
    if (!head || !head->next) return head;
    Q1DateCountNode* middle = Q1FindMiddleDateCount(head);
    Q1DateCountNode* secondHalf = middle->next;
    middle->next = nullptr;
    Q1DateCountNode* left = Q1MergeSortDateCount(head);
    Q1DateCountNode* right = Q1MergeSortDateCount(secondHalf);
    return Q1MergeDateCount(left, right);
}

//===============================================
// QUESTION 2: ELECTRONICS CREDIT CARD PERCENTAGE
//===============================================

// Define a structure for transaction data in Q2
struct Q2Transaction {
    string customerID;
    string product;
    string category;
    double price;
    string date;
    string paymentMethod;
    Q2Transaction* next; // Pointer to the next node in the linked list
    
    // Constructor
    Q2Transaction(string cid, string prod, string cat, double pr, string dt, string pm) :
        customerID(cid), product(prod), category(cat), price(pr), date(dt), paymentMethod(pm), next(nullptr) {}
};

// Define a linked list class to store transactions in Q2
class Q2TransactionList {
private:
    Q2Transaction* head;
    int size;
    
public:
    // Constructor
    Q2TransactionList() : head(nullptr), size(0) {}
    
    // Destructor to free memory
    ~Q2TransactionList() {
        Q2Transaction* current = head;
        while (current != nullptr) {
            Q2Transaction* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    // Add a new transaction to the linked list
    void addTransaction(string cid, string prod, string cat, double pr, string dt, string pm) {
        Q2Transaction* newTransaction = new Q2Transaction(cid, prod, cat, pr, dt, pm);
        
        if (head == nullptr) {
            head = newTransaction;
        } else {
            Q2Transaction* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newTransaction;
        }
        size++;
    }
    
    // Calculate percentage of Electronics purchases made with Credit Card
    void calculateElectronicsCreditCardPercentage() {
        int electronicsCount = 0;
        int creditCardCount = 0;
        
        Q2Transaction* current = head;
        while (current != nullptr) {
            if (current->category == "Electronics") {
                electronicsCount++;
                if (current->paymentMethod == "Credit Card") {
                    creditCardCount++;
                }
            }
            current = current->next;
        }
        
        if (electronicsCount > 0) {
            double percentage = (double)creditCardCount / electronicsCount * 100.0;
            cout << "Percentage of Electronics purchases made with Credit Card: " << endl;
            printf("%.2f%%\n", percentage);  // round to 2 decimal places
        } else {
            cout << "No Electronics purchases found." << endl;
        }
    }
    
    // Getter for size
    int getSize() const {
        return size;
    }
};

//===============================================
// QUESTION 3: WORD FREQUENCY IN 1-STAR REVIEWS
//===============================================

// Node structure for word frequency linked list in Q3
struct Q3WordFreqNode {
    string word;
    int count;
    Q3WordFreqNode* next;

    Q3WordFreqNode(string w, int c) : word(w), count(c), next(nullptr) {}
};

// Linked list class for words in Q3
class Q3WordList {
private:
    Q3WordFreqNode* head;
    int size;

public:
    Q3WordList() : head(nullptr), size(0) {}

    ~Q3WordList() {
        Q3WordFreqNode* current = head;
        while (current != nullptr) {
            Q3WordFreqNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addWord(const string& word, int count) {
        Q3WordFreqNode* newNode = new Q3WordFreqNode(word, count);

        if (head == nullptr) {
            head = newNode;
        } else {
            Q3WordFreqNode* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void incrementOrAdd(const string& word) {
        Q3WordFreqNode* current = head;
        while (current != nullptr) {
            if (current->word == word) {
                current->count++;
                return;
            }
            current = current->next;
        }
        // Not found, add new
        addWord(word, 1);
    }

    Q3WordFreqNode* getHead() const {
        return head;
    }

    void setHead(Q3WordFreqNode* newHead) {
        head = newHead;
    }

    void printTopN(int n) {
        cout << "Top " << n << " most frequent words in 1-star reviews:\n";

        Q3WordFreqNode* current = head;
        int count = 0;

        while (current != nullptr && count < n) {
            cout << current->word << ": " << current->count << endl;
            current = current->next;
            count++;
        }
    }
};

// Node structure for storing individual words during tokenization in Q3
struct Q3WordNode {
    string word;
    Q3WordNode* next;

    Q3WordNode(string w) : word(w), next(nullptr) {}
};

// Linked list for tokenized words in Q3
class Q3TokenList {
private:
    Q3WordNode* head;
    Q3WordNode* tail;
    int size;

public:
    Q3TokenList() : head(nullptr), tail(nullptr), size(0) {}

    ~Q3TokenList() {
        Q3WordNode* current = head;
        while (current != nullptr) {
            Q3WordNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void add(const string& word) {
        Q3WordNode* newNode = new Q3WordNode(word);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    Q3WordNode* getHead() const {
        return head;
    }

    int getSize() const {
        return size;
    }
};

// Tokenizer function for Q3
Q3TokenList Q3Tokenize(string text) {
    Q3TokenList words;
    string word;
    for (char& c : text) {
        if (isalnum(c)) {
            word += tolower(c);
        } else if (!word.empty()) {
            words.add(word);
            word.clear();
        }
    }
    if (!word.empty()) words.add(word);
    return words;
}

// Find middle of linked list in Q3
Q3WordFreqNode* Q3FindMiddle(Q3WordFreqNode* head) {
    if (head == nullptr) return nullptr;

    Q3WordFreqNode* slow = head;
    Q3WordFreqNode* fast = head->next;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// Merge two sorted lists in Q3
Q3WordFreqNode* Q3Merge(Q3WordFreqNode* left, Q3WordFreqNode* right) {
    Q3WordFreqNode* dummy = new Q3WordFreqNode("", 0);
    Q3WordFreqNode* tail = dummy;

    while (left != nullptr && right != nullptr) {
        if (left->count >= right->count) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    if (left != nullptr) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    Q3WordFreqNode* result = dummy->next;
    delete dummy;
    return result;
}

// Merge sort in Q3
Q3WordFreqNode* Q3MergeSort(Q3WordFreqNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Q3WordFreqNode* middle = Q3FindMiddle(head);
    Q3WordFreqNode* secondHalf = middle->next;
    middle->next = nullptr;

    Q3WordFreqNode* left = Q3MergeSort(head);
    Q3WordFreqNode* right = Q3MergeSort(secondHalf);

    return Q3Merge(left, right);
}

//===============================================
// IMPLEMENTATIONS OF EACH QUESTION'S MAIN FUNCTIONS
//===============================================

void runQuestion1() {
    auto total_start_time = high_resolution_clock::now();
    
    // Part 1: Sort transactions by date
    auto transactions_sort_start = high_resolution_clock::now();
    cout << "PART 1: Sorting Transactions by Date" << endl;
    cout << "------------------------------------" << endl;
    
    // Load and sort transactions
    Q1TransactionList transactions = Q1ParseCSV("transactions_cleaned.csv");
    if (transactions.getSize() == 0) {
        std::cerr << "No transactions to sort." << std::endl;
        return;
    }

    cout << "Loaded " << transactions.getSize() << " transactions." << endl;

    // Sort the linked list using merge sort
    Q1Node* sortedHead = Q1MergeSort(transactions.getHead());
    transactions.setHead(sortedHead);
    
    // Write sorted transactions to file
    std::ofstream outFile("sorted_transactions_2.csv");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    outFile << "Customer ID,Product,Category,Price,Date,Payment Method" << std::endl;
    
    Q1Node* current = transactions.getHead();
    while (current != nullptr) {
        outFile << current->data << std::endl;
        current = current->next;
    }
    
    outFile.close();
    
    std::cout << "Transactions sorted and written to sorted_transactions_2.csv" << std::endl;
    cout << endl;

    // Part 2: Process customer reviews and match with transactions
    cout << "PART 2: Match Customer Reviews with Transactions" << endl;
    cout << "----------------------------------------------" << endl;

    Q1CustomerList customerList;
    Q1DateCountList dateCountList;
    string line;

    // Step 1: Load Customer IDs from reviews
    auto reviews_start = high_resolution_clock::now();
    ifstream reviewsFile("reviews_cleaned.csv");
    if (!reviewsFile.is_open()) {
        cerr << "Could not open reviews_cleaned.csv" << endl;
        return;
    }

    int reviewCount = 0;
    getline(reviewsFile, line); // skip header
    while (getline(reviewsFile, line)) {
        string customerID = getColumn(line, 1); // Column 1
        customerList.insert(customerID);
        reviewCount++;
    }
    reviewsFile.close();
    cout << "Loaded " << reviewCount << " customer reviews." << endl;

    // Step 2: Filter transactions by known customers
    auto transactions_match_start = high_resolution_clock::now();
    ifstream transactionsFile("sorted_transactions_2.csv");
    if (!transactionsFile.is_open()) {
        cerr << "Could not open sorted_transactions_2.csv" << endl;
        return;
    }

    int matchedCount = 0;
    getline(transactionsFile, line); // skip header
    while (getline(transactionsFile, line)) {
        string customerID = getColumn(line, 0); // Column 0
        if (customerList.contains(customerID)) {
            string date = getColumn(line, 4); // Column 4
            dateCountList.insertOrIncrement(date);
            matchedCount++;
        }
    }
    transactionsFile.close();
    cout << "Found " << matchedCount << " matching transactions." << endl;

    // Step 3: Sort the date-count list
    auto sorting_start = high_resolution_clock::now();
    Q1DateCountNode* sortedDateCountHead = Q1MergeSortDateCount(dateCountList.getHead());
    dateCountList.setHead(sortedDateCountHead);

    // Step 4: Display the results
    cout << "\nMatched Transactions by Date:" << endl;
    cout << left << setw(5) << "#" << setw(15) << "Date" << setw(25) << "Matched Transactions" << endl;
    cout << string(45, '-') << endl;

    int total = 0, count = 1;
    Q1DateCountNode* currentDateCount = dateCountList.getHead();
    while (currentDateCount) {
        cout << left << setw(5) << to_string(count++) + ")"
             << setw(15) << currentDateCount->date
             << setw(25) << currentDateCount->count << endl;
        total += currentDateCount->count;
        currentDateCount = currentDateCount->next;
    }

    cout << string(45, '-') << endl;
    cout << left << setw(5) << "" << setw(15) << "Total" << setw(25) << total << endl;

    // Final execution time stats
    auto end_time = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(end_time - total_start_time);

    cout << "\nExecution Times:" << endl;
    cout << "----------------" << endl;
    cout << "Total execution time:    " << total_duration.count() << " ms" << endl;
}

void runQuestion2() {
    ifstream file("transactions_cleaned.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header
    
    Q2TransactionList transactions; // Create a linked list to store transactions

    // Start timing
    auto start = high_resolution_clock::now();
    
    // Read and parse the CSV file
    while (getline(file, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;
        
        // Parse the CSV line
        getline(ss, customerID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod);
        
        // Convert price string to double
        double price = 0.0;
        try {
            price = stod(priceStr);
        } catch (const exception& e) {
            cerr << "Error converting price: " << priceStr << endl;
        }
        
        // Add transaction to the linked list
        transactions.addTransaction(customerID, product, category, price, date, paymentMethod);
    }
    
    file.close();
    
    // Calculate percentage using linked list
    transactions.calculateElectronicsCreditCardPercentage();

    // End timing
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Total transactions processed: " << transactions.getSize() << endl;
    cout << "Time taken using linked list: " << duration.count() << " milliseconds" << endl;
}

void runQuestion3() {
    auto start_time = high_resolution_clock::now();

    ifstream file("reviews_cleaned.csv");
    string line;
    bool headerSkipped = false;

    Q3WordList wordFreq;

    while (getline(file, line)) {
        if (!headerSkipped) { headerSkipped = true; continue; }

        stringstream ss(line);
        string productId, customerId, ratingStr, review;
        getline(ss, productId, ',');
        getline(ss, customerId, ',');
        getline(ss, ratingStr, ',');
        getline(ss, review);

        int rating = stoi(ratingStr);

        if (rating == 1) {
            Q3TokenList words = Q3Tokenize(review);
            Q3WordNode* current = words.getHead();
            while (current != nullptr) {
                wordFreq.incrementOrAdd(current->word);
                current = current->next;
            }
        }
    }

    Q3WordFreqNode* sortedHead = Q3MergeSort(wordFreq.getHead());
    wordFreq.setHead(sortedHead);

    wordFreq.printTopN(10);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "\nExecution time: " << duration.count() << " milliseconds" << endl;
}

//===============================================
// MAIN FUNCTION
//===============================================

int main() {
    int choice;
    
    while (true) {
        cout << "\n===============================================" << endl;
        cout << "            COMBINED PROGRAM MENU              " << endl;
        cout << "===============================================" << endl;
        cout << "1. Run Question 1 (Sort Transactions & Match with Reviews)" << endl;
        cout << "2. Run Question 2 (Electronics Credit Card Percentage)" << endl;
        cout << "3. Run Question 3 (Word Frequency in 1-Star Reviews)" << endl;
        cout << "4. Run All Questions" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        if (choice == 0) {
            cout << "Exiting program..." << endl;
            break;
        }
        
        cout << endl;
        switch (choice) {
            case 1:
                cout << "\n--- Running Question 1 ---\n";
                runQuestion1();
                break;
            case 2:
                cout << "\n--- Running Question 2 ---\n";
                runQuestion2();
                break;
            case 3:
                cout << "\n--- Running Question 3 ---\n";
                runQuestion3();
                break;
            case 4:
                cout << "\n--- Running All Questions ---\n";
                runQuestion1();
                runQuestion2();
                runQuestion3();
                break;
            default:
                cout << "Invalid choice. Please enter a number between 0 and 4." << endl;
        }
    }

    return 0;
}