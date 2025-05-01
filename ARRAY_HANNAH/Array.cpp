#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

//=============================================================================
// COMMON FUNCTIONS AND STRUCTURES
//=============================================================================

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

//=============================================================================
// QUESTION 1 CODE
//=============================================================================

// Define constants for array sizes
const int MAX_TRANSACTIONS = 10000;
const int MAX_CUSTOMERS = 5000;
const int MAX_DATE_COUNTS = 1000;

// Function to convert date format from DD/MM/YYYY to YYYY-MM-DD
string convertDateFormat(const string& date) {
    stringstream ss(date);
    string day, month, year;
    getline(ss, day, '/');
    getline(ss, month, '/');
    getline(ss, year, '/');
    return year + "-" + month + "-" + day;
}

// Struct for transaction details
struct Transaction {
    string customerID;
    string product;
    string category;
    double price;
    string date;
    string paymentMethod;

    Transaction() : customerID(""), product(""), category(""), price(0.0), date(""), paymentMethod("") {}

    Transaction(string cid, string prod, string cat, double pr, string dt, string pm)
        : customerID(cid), product(prod), category(cat), price(pr), date(dt), paymentMethod(pm) {}

    friend ostream& operator<<(ostream& os, const Transaction& t) {
        os << t.customerID << "," << t.product << "," << t.category << ","
           << t.price << "," << t.date << "," << t.paymentMethod;
        return os;
    }
};

// Array-based implementation for transactions
class TransactionArray {
private:
    Transaction* transactions;
    int capacity;
    int size;

public:
    TransactionArray(int cap = MAX_TRANSACTIONS) : capacity(cap), size(0) {
        transactions = new Transaction[capacity];
    }

    ~TransactionArray() {
        delete[] transactions;
    }

    void push_back(const Transaction& t) {
        if (size < capacity) {
            transactions[size++] = t;
        }
    }

    int getSize() const {
        return size;
    }

    Transaction* getTransactions() {
        return transactions;
    }
};

// Structure for date-count pairs
struct DateCount {
    string date;
    int count;
    
    DateCount() : date(""), count(0) {}
    DateCount(string d, int c) : date(d), count(c) {}
};

// Array-based implementation for customer IDs
class CustomerArray {
private:
    string* customerIDs;
    int capacity;
    int size;

public:
    CustomerArray(int cap = MAX_CUSTOMERS) : capacity(cap), size(0) {
        customerIDs = new string[capacity];
    }

    ~CustomerArray() {
        delete[] customerIDs;
    }

    void insert(const string& id) {
        if (!contains(id) && size < capacity) {
            customerIDs[size++] = id;
        }
    }

    bool contains(const string& id) const {
        for (int i = 0; i < size; i++) {
            if (customerIDs[i] == id) return true;
        }
        return false;
    }
    
    int getSize() const {
        return size;
    }
};

// Array-based implementation for date-count pairs
class DateCountArray {
private:
    DateCount* dateCounts;
    int capacity;
    int size;

public:
    DateCountArray(int cap = MAX_DATE_COUNTS) : capacity(cap), size(0) {
        dateCounts = new DateCount[capacity];
    }

    ~DateCountArray() {
        delete[] dateCounts;
    }

    void insertOrIncrement(const string& date) {
        // Check if date exists
        for (int i = 0; i < size; i++) {
            if (dateCounts[i].date == date) {
                dateCounts[i].count++;
                return;
            }
        }
        
        // If date doesn't exist and we have space, add it
        if (size < capacity) {
            dateCounts[size++] = DateCount(date, 1);
        }
    }

    DateCount* getDateCounts() {
        return dateCounts;
    }
    
    int getSize() const {
        return size;
    }
};

// Parse CSV file for transactions
TransactionArray parseCSV(const string& filename) {
    TransactionArray transactions;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return transactions;
    }

    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;

        getline(ss, customerID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod);

        double price = stod(priceStr);
        date = convertDateFormat(date);

        transactions.push_back(Transaction(customerID, product, category, price, date, paymentMethod));
    }
    file.close();
    return transactions;
}

// Merge Sort for Transaction array
void merge(Transaction arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temp arrays
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];
    
    // Copy data to temp arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temp arrays back into arr[left..right]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray
    
    while (i < n1 && j < n2) {
        if (L[i].date <= R[j].date) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

// Merge Sort helper function
void mergeSort(Transaction arr[], int left, int right) {
    if (left < right) {
        // Same as (left+right)/2, but avoids overflow
        int mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Merge Sort for DateCount array
void mergeDateCount(DateCount arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    DateCount* L = new DateCount[n1];
    DateCount* R = new DateCount[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i].date <= R[j].date) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

void mergeSortDateCount(DateCount arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortDateCount(arr, left, mid);
        mergeSortDateCount(arr, mid + 1, right);
        mergeDateCount(arr, left, mid, right);
    }
}

// Question 1 main function
void question1() {
    auto start_time = high_resolution_clock::now();
    
    cout << "Starting transaction processing system..." << endl;
    cout << "------------------------------------------" << endl;
    
    // Step 1: Load Customer IDs from reviews
    
    CustomerArray customerArray;
    string line;
    ifstream reviewsFile("reviews_cleaned.csv");
    if (!reviewsFile.is_open()) {
        cerr << "Could not open reviews_cleaned.csv" << endl;
        return;
    }

    int reviewCount = 0;
    getline(reviewsFile, line); // skip header
    while (getline(reviewsFile, line)) {
        string customerID = getColumn(line, 1); // Column 1
        customerArray.insert(customerID);
        reviewCount++;
    }
    reviewsFile.close();
    
    cout << "Loaded " << reviewCount << " customer reviews." << endl;
    
    // Step 2: Load all transactions
    
    TransactionArray allTransactions = parseCSV("transactions_cleaned.csv");
    if (allTransactions.getSize() == 0) {
        cerr << "No transactions found." << endl;
        return;
    }
    
    cout << "Loaded " << allTransactions.getSize() << " transactions." << endl;
    
    // Step 3: Sort all transactions
    
    Transaction* transactionsArr = allTransactions.getTransactions();
    mergeSort(transactionsArr, 0, allTransactions.getSize() - 1);
        
    // Step 4: Write all sorted transactions to file
    
    ofstream outFile("sorted_transactions_2.csv");
    if (!outFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return;
    }
    
    outFile << "Customer ID,Product,Category,Price,Date,Payment Method" << endl;
    for (int i = 0; i < allTransactions.getSize(); i++) {
        outFile << transactionsArr[i] << endl;
    }
    outFile.close();
    
    
    // Step 5: Filter and count transactions from customers who left reviews
    
    DateCountArray dateCountArray;
    
    // Read the sorted transactions file we just created
    ifstream sortedFile("sorted_transactions_2.csv");
    if (!sortedFile.is_open()) {
        cerr << "Could not open sorted_transactions_2.csv" << endl;
        return;
    }
    
    int matchedCount = 0;
    getline(sortedFile, line); // skip header
    while (getline(sortedFile, line)) {
        string customerID = getColumn(line, 0); // Column 0
        if (customerArray.contains(customerID)) {
            string date = getColumn(line, 4); // Column 4
            dateCountArray.insertOrIncrement(date);
            matchedCount++;
        }
    }
    sortedFile.close();
    
    // Step 6: Sort date-count pairs
    DateCount* dateCounts = dateCountArray.getDateCounts();
    mergeSortDateCount(dateCounts, 0, dateCountArray.getSize() - 1);
    
    // Step 7: Display results
    cout << "\nMatched Transactions by Date:" << endl;
    cout << left << setw(5) << "#" << setw(15) << "Date" << setw(25) << "Matched Transactions" << endl;
    cout << string(45, '-') << endl;
    
    int total = 0, count = 1;
    for (int i = 0; i < dateCountArray.getSize(); i++) {
        cout << left << setw(5) << to_string(count++) + ")"
             << setw(15) << dateCounts[i].date
             << setw(25) << dateCounts[i].count << endl;
        total += dateCounts[i].count;
    }
    
    cout << string(45, '-') << endl;
    cout << left << setw(5) << "" << setw(15) << "Total" << setw(25) << total << endl;
    
    // Step 8: Write filtered transactions to a separate file (additional functionality from paste.txt)
    cout << "\nWriting filtered transactions to file..." << endl;
    
    ofstream filteredFile("sorted_transactions.csv");
    if (!filteredFile.is_open()) {
        cerr << "Could not open sorted_transactions.csv for writing" << endl;
        return;
    }
    
    filteredFile << "CustomerID,Product,Category,Price,Date,PaymentMethod" << endl;
    
    // Re-read sorted transactions and write only the filtered ones
    sortedFile.open("sorted_transactions_2.csv");
    if (!sortedFile.is_open()) {
        cerr << "Could not re-open sorted_transactions_2.csv" << endl;
        return;
    }
    
    getline(sortedFile, line); // skip header
    while (getline(sortedFile, line)) {
        string customerID = getColumn(line, 0);
        if (customerArray.contains(customerID)) {
            filteredFile << line << endl;
        }
    }
    sortedFile.close();
    filteredFile.close();
     
    // Final execution time stats
    auto end_time = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(end_time - start_time);
    
    cout << "\nExecution Times:" << endl;
    cout << "----------------" << endl;
    cout << "Total execution time:     " << total_duration.count() << " ms" << endl;
}

//=============================================================================
// QUESTION 2 CODE
//=============================================================================

void question2() {
    ifstream file("transactions_cleaned.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return;
    }

    string categories[MAX_TRANSACTIONS];
    string paymentMethods[MAX_TRANSACTIONS];
    int totalRecords = 0;

    string line;
    getline(file, line); // Skip the header

    // Reading the file and storing into arrays
    while (getline(file, line) && totalRecords < MAX_TRANSACTIONS) {
        stringstream ss(line);
        string cell;
        string category, paymentMethod;
        int col = 0;
        while (getline(ss, cell, ',')) {
            if (col == 2) category = cell;
            else if (col == 5) paymentMethod = cell;
            col++;
        }
        categories[totalRecords] = category;
        paymentMethods[totalRecords] = paymentMethod;
        totalRecords++;
    }

    file.close();

    int electronicsCount = 0;
    int creditCardCount = 0;

    // Start timing
    auto start_time = high_resolution_clock::now();

    // Linear search through the arrays
    for (int i = 0; i < totalRecords; i++) {
        if (categories[i] == "Electronics") {
            electronicsCount++;
            if (paymentMethods[i] == "Credit Card") {
                creditCardCount++;
            }
        }
    }

    // End timing
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    if (electronicsCount > 0) {
        double percentage = (double)creditCardCount / electronicsCount * 100.0;
        cout << "Percentage of Electronics purchases made with Credit Card:" << endl;
        printf("%.2f%%\n", percentage);
    } else {
        cout << "No Electronics purchases found." << endl;
    }

    cout << "\nTime taken for linear search: " << duration.count() << " milliseconds" << endl;
}

//=============================================================================
// QUESTION 3 CODE
//=============================================================================

const int MAX_WORDS = 10000; // Maximum number of unique words we can store

struct WordCount {
    string word;
    int count;
};

// Merge function for Merge Sort
void merge(WordCount arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    WordCount* L = new WordCount[n1];
    WordCount* R = new WordCount[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].count > R[j].count) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Merge Sort function
void mergeSort(WordCount arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Helper function to tokenize and clean review text
int tokenize(const string& text, string words[]) {
    int wordIndex = 0;
    string word = "";

    for (char c : text) {
        if (isalnum(c)) {
            word += tolower(c);
        } else if (!word.empty()) {
            if (wordIndex < MAX_WORDS) {
                words[wordIndex++] = word;
            }
            word.clear();
        }
    }
    if (!word.empty() && wordIndex < MAX_WORDS) {
        words[wordIndex++] = word;
    }
    return wordIndex;
}

void question3() {
    auto start_time = high_resolution_clock::now();

    ifstream file("reviews_cleaned.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return;
    }

    WordCount wordCounts[MAX_WORDS];
    int uniqueWords = 0;
    bool headerSkipped = false;
    string line;

    // Read reviews and count words
    while (getline(file, line)) {
        if (!headerSkipped) { headerSkipped = true; continue; }

        stringstream ss(line);
        string productId, customerId, ratingStr, review;
        getline(ss, productId, ',');
        getline(ss, customerId, ',');
        getline(ss, ratingStr, ',');
        getline(ss, review);

        int rating = stoi(ratingStr);

        if (rating == 1) { // Only 1-star reviews
            string wordsInReview[MAX_WORDS];
            int numWords = tokenize(review, wordsInReview);

            for (int i = 0; i < numWords; i++) {
                string word = wordsInReview[i];
                bool found = false;
                //Linear search for the word in the array
                for (int j = 0; j < uniqueWords; j++) {
                    if (wordCounts[j].word == word) {
                        wordCounts[j].count++;
                        found = true;
                        break;
                    }
                }
                if (!found && uniqueWords < MAX_WORDS) {
                    wordCounts[uniqueWords].word = word;
                    wordCounts[uniqueWords].count = 1;
                    uniqueWords++;
                }
            }
        }
    }

    file.close();

    // Apply Merge Sort
    mergeSort(wordCounts, 0, uniqueWords - 1);

    // Output Top 10 frequent words
    cout << "Top 10 most frequent words in 1-star reviews:\n";
    int top = (uniqueWords < 10) ? uniqueWords : 10;
    for (int i = 0; i < top; i++) {
        cout << wordCounts[i].word << ": " << wordCounts[i].count << endl;
    }

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "\nExecution Times:" << endl;
    cout << "----------------" << endl;
    cout << "Total Execution time: " << duration.count() << " milliseconds" << endl;
}

//=============================================================================
// MAIN FUNCTION
//=============================================================================

int main() {
    cout << "==============================================================================" << endl;
    cout << "                            QUESTION 1                                        " << endl;
    cout << "==============================================================================" << endl;
    question1();

    cout << "\n\n==============================================================================" << endl;
    cout << "                            QUESTION 2                                        " << endl;
    cout << "==============================================================================" << endl;
    question2();

    cout << "\n\n==============================================================================" << endl;
    cout << "                            QUESTION 3                                        " << endl;
    cout << "==============================================================================" << endl;
    question3();

    return 0;
}