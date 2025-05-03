#include "ecommerceAnalyzer.hpp"
#include <set>

using namespace std;
using namespace std::chrono;

// ------------------- Constructor Implementations -------------------
Transaction::Transaction() : 
    customerID(""), product(""), category(""), price(0.0), date(""), paymentMethod(""), next(nullptr) {}

Transaction::Transaction(const string& cid, const string& prod, const string& cat, 
                       double pr, const string& dt, const string& pm) :
    customerID(cid), product(prod), category(cat), price(pr), date(dt), paymentMethod(pm), next(nullptr) {}

Review::Review() : 
    productID(""), customerID(""), rating(0), reviewText(""), next(nullptr) {}

Review::Review(const string& pid, const string& cid, int r, const string& text) :
    productID(pid), customerID(cid), rating(r), reviewText(text), next(nullptr) {}

DateCountNode::DateCountNode(const string& d, int c) : 
    date(d), count(c), next(nullptr) {}

WordFrequency::WordFrequency(const string& w, int f) : 
    word(w), frequency(f), next(nullptr) {}

// ------------------- Utility Functions -------------------
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string convertDateFormat(const string& date) {
    // Check if date is already in YYYY-MM-DD format
    if (date.size() >= 10 && date[4] == '-' && date[7] == '-') {
        return date;
    }
    
    // Handle DD/MM/YYYY format
    if (date.size() >= 10 && (date[2] == '/' || date[2] == '-')) {
        string day = date.substr(0, 2);
        string month = date.substr(3, 2);
        string year = date.substr(6, 4);
        return year + "-" + month + "-" + day;
    }
    
    // Return original if no conversion is possible
    return date;
}

bool isDateLess(const string& date1, const string& date2) {
    return date1 < date2;
}

string cleanWord(const string& word) {
    string result;
    for (char ch : word) {
        if (isalpha(ch)) {
            result += tolower(ch);
        }
    }
    return result;
}

// ------------------- Linked List Operations -------------------
void insertTransaction(Transaction*& head, const Transaction& t) {
    Transaction* newNode = new Transaction(t);
    
    if (head == nullptr) {
        head = newNode;
    } else {
        Transaction* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void insertTransaction(Transaction*& head, const string& customerID, const string& product, 
                      const string& category, double price, const string& date, 
                      const string& paymentMethod) {
    Transaction* newNode = new Transaction(customerID, product, category, price, date, paymentMethod);
    
    if (head == nullptr) {
        head = newNode;
    } else {
        Transaction* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void insertReview(Review*& head, const string& productID, const string& customerID, 
                 int rating, const string& reviewText) {
    Review* newNode = new Review(productID, customerID, rating, reviewText);
    
    if (head == nullptr) {
        head = newNode;
    } else {
        Review* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void insertDateCount(DateCountNode*& head, const string& date) {
    // Check if date already exists in list
    DateCountNode* current = head;
    while (current) {
        if (current->date == date) {
            current->count++;
            return;
        }
        current = current->next;
    }
    
    // If date doesn't exist, create new node
    DateCountNode* newNode = new DateCountNode(date, 1);
    
    // Insert in sorted order by date
    if (head == nullptr || date < head->date) {
        newNode->next = head;
        head = newNode;
    } else {
        current = head;
        while (current->next && current->next->date < date) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void insertWordFrequency(WordFrequency*& head, const string& word, int frequency) {
    WordFrequency* newNode = new WordFrequency(word, frequency);
    
    // Insert in sorted order by frequency (highest first)
    if (head == nullptr || frequency > head->frequency) {
        newNode->next = head;
        head = newNode;
    } else {
        WordFrequency* current = head;
        while (current->next && current->next->frequency >= frequency) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void incrementWordFrequency(WordFrequency*& head, const string& word) {
    // Check if word already exists
    WordFrequency* current = head;
    while (current) {
        if (current->word == word) {
            current->frequency++;
            return;
        }
        current = current->next;
    }
    
    // Word not found, add it with frequency 1
    insertWordFrequency(head, word, 1);
}

void insertCustomerID(CustomerIDNode*& head, const string& customerID) {
    // Check if ID already exists
    CustomerIDNode* current = head;
    while (current) {
        if (current->customerID == customerID) {
            return; // ID already exists, don't add duplicate
        }
        current = current->next;
    }
    
    // Insert new ID
    CustomerIDNode* newNode = new CustomerIDNode(customerID);
    newNode->next = head;
    head = newNode;
}

bool isCustomerInList(CustomerIDNode* head, const string& customerID) {
    CustomerIDNode* current = head;
    while (current) {
        if (current->customerID == customerID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void freeCustomerIDList(CustomerIDNode*& head) {
    CustomerIDNode* current = head;
    while (current) {
        CustomerIDNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

// ------------------- Memory Management -------------------
void freeTransactionList(Transaction*& head) {
    Transaction* current = head;
    while (current) {
        Transaction* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void freeReviewList(Review*& head) {
    Review* current = head;
    while (current) {
        Review* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void freeDateCountList(DateCountNode*& head) {
    DateCountNode* current = head;
    while (current) {
        DateCountNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void freeWordFrequencyList(WordFrequency*& head) {
    WordFrequency* current = head;
    while (current) {
        WordFrequency* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

// ------------------- File Operations -------------------
Transaction* loadTransactionsFromCSV(const string& filename, int& count) {
    Transaction* head = nullptr;
    count = 0;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return nullptr;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    int lineNum = 1;
    while (getline(file, line)) {
        lineNum++;
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;
        
        try {
            // Parse CSV line
            getline(ss, customerID, ',');
            getline(ss, product, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, date, ',');
            getline(ss, paymentMethod);
            
            // Trim whitespace
            customerID = trim(customerID);
            product = trim(product);
            category = trim(category);
            priceStr = trim(priceStr);
            date = trim(date);
            paymentMethod = trim(paymentMethod);
            
            // Convert price to double
            double price = 0.0;
            try {
                price = stod(priceStr);
            } catch (const exception& e) {
                cerr << "Warning: Invalid price '" << priceStr << "' at line " << lineNum << endl;
                continue;
            }
            
            // Convert date format if needed
            date = convertDateFormat(date);
            
            // Add transaction to list
            insertTransaction(head, customerID, product, category, price, date, paymentMethod);
            count++;
        } catch (const exception& e) {
            cerr << "Error: Failed to parse line " << lineNum << ": " << e.what() << endl;
            continue;
        }
    }
    
    file.close();
    return head;
}

Review* loadReviewsFromCSV(const string& filename, int& count) {
    Review* head = nullptr;
    count = 0;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return nullptr;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    int lineNum = 1;
    while (getline(file, line)) {
        lineNum++;
        stringstream ss(line);
        string productID, customerID, ratingStr, reviewText;
        
        try {
            // Parse CSV line
            getline(ss, productID, ',');
            getline(ss, customerID, ',');
            getline(ss, ratingStr, ',');
            getline(ss, reviewText);
            
            // Trim whitespace
            productID = trim(productID);
            customerID = trim(customerID);
            ratingStr = trim(ratingStr);
            // Don't trim review text to preserve its content
            
            // Convert rating to int
            int rating = 0;
            try {
                rating = stoi(ratingStr);
            } catch (const exception& e) {
                cerr << "Warning: Invalid rating '" << ratingStr << "' at line " << lineNum << endl;
                continue;
            }
            
            // Add review to list
            insertReview(head, productID, customerID, rating, reviewText);
            count++;
        } catch (const exception& e) {
            cerr << "Error: Failed to parse line " << lineNum << ": " << e.what() << endl;
            continue;
        }
    }
    
    file.close();
    return head;
}

// ------------------- Sorting Algorithms -------------------
// 1. Merge Sort Implementation
Transaction* findMiddle(Transaction* head) {
    if (!head) return nullptr;
    
    Transaction* slow = head;
    Transaction* fast = head->next;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

Transaction* merge(Transaction* left, Transaction* right) {
    if (!left) return right;
    if (!right) return left;
    
    Transaction* result = nullptr;
    
    // Choose head based on date comparison
    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

Transaction* mergeSort(Transaction* head) {
    if (!head || !head->next) return head;
    
    // Find middle of list
    Transaction* middle = findMiddle(head);
    Transaction* right = middle->next;
    middle->next = nullptr;
    
    // Sort left and right halves
    Transaction* left = mergeSort(head);
    right = mergeSort(right);
    
    // Merge sorted halves
    return merge(left, right);
}

// 2. Bubble Sort Implementation
void bubbleSort(Transaction*& head) {
    if (!head || !head->next) return;
    
    bool swapped;
    Transaction* ptr1;
    Transaction* lptr = nullptr;
    
    do {
        swapped = false;
        ptr1 = head;
        
        while (ptr1->next != lptr) {
            if (ptr1->date > ptr1->next->date) {
                // Swap data
                string temp;
                double tempPrice;
                
                temp = ptr1->customerID;
                ptr1->customerID = ptr1->next->customerID;
                ptr1->next->customerID = temp;
                
                temp = ptr1->product;
                ptr1->product = ptr1->next->product;
                ptr1->next->product = temp;
                
                temp = ptr1->category;
                ptr1->category = ptr1->next->category;
                ptr1->next->category = temp;
                
                tempPrice = ptr1->price;
                ptr1->price = ptr1->next->price;
                ptr1->next->price = tempPrice;
                
                temp = ptr1->date;
                ptr1->date = ptr1->next->date;
                ptr1->next->date = temp;
                
                temp = ptr1->paymentMethod;
                ptr1->paymentMethod = ptr1->next->paymentMethod;
                ptr1->next->paymentMethod = temp;
                
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// 3. Insertion Sort Implementation
void insertionSort(Transaction*& head) {
    if (!head || !head->next) return;
    
    Transaction* sorted = nullptr;
    Transaction* current = head;
    
    while (current) {
        Transaction* next = current->next;
        
        // Insert current node into sorted linked list
        if (!sorted || current->date < sorted->date) {
            current->next = sorted;
            sorted = current;
        } else {
            Transaction* temp = sorted;
            while (temp->next && temp->next->date < current->date) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        
        current = next;
    }
    
    head = sorted;
}

// 4. Quick Sort Implementation
Transaction* getLastNode(Transaction* head) {
    if (!head) return nullptr;
    
    while (head->next) {
        head = head->next;
    }
    
    return head;
}

void swapTransactionData(Transaction* a, Transaction* b) {
    string temp;
    double tempPrice;
    
    temp = a->customerID;
    a->customerID = b->customerID;
    b->customerID = temp;
    
    temp = a->product;
    a->product = b->product;
    b->product = temp;
    
    temp = a->category;
    a->category = b->category;
    b->category = temp;
    
    tempPrice = a->price;
    a->price = b->price;
    b->price = tempPrice;
    
    temp = a->date;
    a->date = b->date;
    b->date = temp;
    
    temp = a->paymentMethod;
    a->paymentMethod = b->paymentMethod;
    b->paymentMethod = temp;
}

Transaction* partition(Transaction* start, Transaction* end) {
    // Select pivot as end node
    string pivotDate = end->date;
    
    Transaction* i = start;
    Transaction* j = start;
    
    while (j != end) {
        if (j->date <= pivotDate) {
            swapTransactionData(i, j);
            i = i->next;
        }
        j = j->next;
    }
    
    swapTransactionData(i, end);
    return i;
}

void improvedQuickSort(Transaction* start, Transaction* end) {
    if (start == nullptr || end == nullptr || start == end) {
        return;
    }
    
    // Static counter for progress tracking
    static int operations = 0;
    static int lastReported = 0;
    operations++;
    
    // Report progress every 1000 operations
    if (operations - lastReported >= 1000) {
        cout << "QuickSort: " << operations << " operations completed...\r";
        cout.flush();
        lastReported = operations;
    }
    
    Transaction* pivot = partition(start, end);
    
    // Recursively sort the left part
    if (start != pivot) {
        // Find node before pivot
        Transaction* temp = start;
        while (temp->next != pivot) {
            temp = temp->next;
        }
        improvedQuickSort(start, temp);
    }
    
    // Recursively sort the right part
    if (pivot != end && pivot->next) {
        improvedQuickSort(pivot->next, end);
    }
}

// ------------------- Searching Algorithms -------------------
// 1. Linear Search
int linearSearch(Transaction* head, const string& category, const string& paymentMethod, 
                int& totalCount, int& matchCount) {
    totalCount = 0;
    matchCount = 0;
    Transaction* current = head;
    
    while (current) {
        if (current->category == category) {
            totalCount++;
            if (current->paymentMethod == paymentMethod) {
                matchCount++;
            }
        }
        current = current->next;
    }
    
    return (totalCount > 0) ? (matchCount * 100 / totalCount) : 0;
}

// 2. Binary Search
// Note: For binary search, the list needs to be sorted by category first
int binarySearch(Transaction* head, const string& category, const string& paymentMethod, 
                int& totalCount, int& matchCount) {
    totalCount = 0;
    matchCount = 0;
    
    // First, traverse the list to count the nodes
    int nodeCount = 0;
    Transaction* current = head;
    while (current) {
        nodeCount++;
        current = current->next;
    }
    
    // For binary search to be efficient, we'll create an array of pointers to transactions
    // This is a workaround since linked lists don't support random access
    Transaction** transArray = new Transaction*[nodeCount];
    current = head;
    for (int i = 0; i < nodeCount; i++) {
        transArray[i] = current;
        current = current->next;
    }
    
    // Sort array by category (simple bubble sort for demonstration)
    for (int i = 0; i < nodeCount - 1; i++) {
        for (int j = 0; j < nodeCount - i - 1; j++) {
            if (transArray[j]->category > transArray[j+1]->category) {
                Transaction* temp = transArray[j];
                transArray[j] = transArray[j+1];
                transArray[j+1] = temp;
            }
        }
    }
    
    // Perform binary search to find the start of the category
    int left = 0;
    int right = nodeCount - 1;
    int firstIndex = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (transArray[mid]->category == category) {
            firstIndex = mid;
            // Continue searching left to find the first occurrence
            right = mid - 1;
        } else if (transArray[mid]->category < category) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    // If category was found, count occurrences and check payment method
    if (firstIndex != -1) {
        for (int i = firstIndex; i < nodeCount && transArray[i]->category == category; i++) {
            totalCount++;
            if (transArray[i]->paymentMethod == paymentMethod) {
                matchCount++;
            }
        }
    }
    
    // Clean up
    delete[] transArray;
    
    return (totalCount > 0) ? (matchCount * 100 / totalCount) : 0;
}

// 3. Jump Search
// Improved Jump Search for Linked Lists
int jumpSearch(Transaction* head, const string& category, const string& paymentMethod, 
              int& totalCount, int& matchCount) {
    totalCount = 0;
    matchCount = 0;
    
    if (!head) return 0;
    
    // Count the nodes first (required for jump size calculation)
    int nodeCount = 0;
    Transaction* current = head;
    while (current) {
        nodeCount++;
        current = current->next;
    }
    
    // Calculate jump size - square root of list length is optimal
    int jumpSize = sqrt(nodeCount);
    
    // Create a temporary array of pointers for category-based search
    Transaction** transArray = new Transaction*[nodeCount];
    current = head;
    for (int i = 0; i < nodeCount; i++) {
        transArray[i] = current;
        current = current->next;
    }
    
    // Sort the pointers by category for jump search to work properly
    for (int i = 0; i < nodeCount - 1; i++) {
        for (int j = 0; j < nodeCount - i - 1; j++) {
            if (transArray[j]->category > transArray[j+1]->category) {
                Transaction* temp = transArray[j];
                transArray[j] = transArray[j+1];
                transArray[j+1] = temp;
            }
        }
    }
    
    // Now perform jump search on the sorted array
    int i = 0;
    
    // Jump forward
    while (i < nodeCount && transArray[i]->category < category) {
        i = min(i + jumpSize, nodeCount - 1);
    }
    
    // Backtrack to find the first occurrence
    int start = i - jumpSize;
    if (start < 0) start = 0;
    
    // Linear search from the start position
    for (int j = start; j <= i && j < nodeCount; j++) {
        if (transArray[j]->category == category) {
            totalCount++;
            if (transArray[j]->paymentMethod == paymentMethod) {
                matchCount++;
            }
        }
    }
    
    // Continue searching forward for all occurrences
    for (int j = i + 1; j < nodeCount && transArray[j]->category == category; j++) {
        totalCount++;
        if (transArray[j]->paymentMethod == paymentMethod) {
            matchCount++;
        }
    }
    
    // Clean up
    delete[] transArray;
    
    return (totalCount > 0) ? (matchCount * 100 / totalCount) : 0;
}

// Improved Exponential Search for Linked Lists
int exponentialSearch(Transaction* head, const string& category, const string& paymentMethod, 
                     int& totalCount, int& matchCount) {
    totalCount = 0;
    matchCount = 0;
    
    if (!head) return 0;
    
    // Count the nodes and create an array of pointers (necessary for exponential search)
    int nodeCount = 0;
    Transaction* current = head;
    while (current) {
        nodeCount++;
        current = current->next;
    }
    
    // Create a temporary array of pointers
    Transaction** transArray = new Transaction*[nodeCount];
    current = head;
    for (int i = 0; i < nodeCount; i++) {
        transArray[i] = current;
        current = current->next;
    }
    
    // Sort the pointers by category
    for (int i = 0; i < nodeCount - 1; i++) {
        for (int j = 0; j < nodeCount - i - 1; j++) {
            if (transArray[j]->category > transArray[j+1]->category) {
                Transaction* temp = transArray[j];
                transArray[j] = transArray[j+1];
                transArray[j+1] = temp;
            }
        }
    }
    
    // Exponential search
    int bound = 1;
    while (bound < nodeCount && transArray[bound-1]->category < category) {
        bound *= 2;
    }
    
    // Binary search within the identified range
    int left = bound / 2;
    int right = min(bound, nodeCount) - 1;
    int firstOccurrence = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (transArray[mid]->category == category) {
            firstOccurrence = mid;
            right = mid - 1; // Look for earlier occurrences
        } else if (transArray[mid]->category < category) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    // If found, count all occurrences
    if (firstOccurrence != -1) {
        for (int i = firstOccurrence; i < nodeCount && transArray[i]->category == category; i++) {
            totalCount++;
            if (transArray[i]->paymentMethod == paymentMethod) {
                matchCount++;
            }
        }
    }
    
    // Clean up
    delete[] transArray;
    
    return (totalCount > 0) ? (matchCount * 100 / totalCount) : 0;
}

// ------------------- Analysis Functions -------------------
DateCountNode* countTransactionsByDate(Transaction* head) {
    DateCountNode* dateCountHead = nullptr;
    
    Transaction* current = head;
    while (current) {
        insertDateCount(dateCountHead, current->date);
        current = current->next;
    }
    
    return dateCountHead;
}

double calculateCategoryPaymentPercentage(Transaction* head, const string& category, 
                                        const string& paymentMethod, int algorithm) {
    int totalCount = 0;
    int matchCount = 0;
    
    switch (algorithm) {
        case 1: // Linear Search
            linearSearch(head, category, paymentMethod, totalCount, matchCount);
            break;
        case 2: // Binary Search
            binarySearch(head, category, paymentMethod, totalCount, matchCount);
            break;
        case 3: // Jump Search
            jumpSearch(head, category, paymentMethod, totalCount, matchCount);
            break;
        case 4: // Exponential Search
            exponentialSearch(head, category, paymentMethod, totalCount, matchCount);
            break;
        default:
            cout << "Invalid algorithm selection. Using Linear Search." << endl;
            linearSearch(head, category, paymentMethod, totalCount, matchCount);
    }
    
    double percentage = (totalCount > 0) ? (static_cast<double>(matchCount) / totalCount * 100.0) : 0.0;
    
    cout << "Total " << category << " purchases: " << totalCount << endl;
    cout << category << " purchases made with " << paymentMethod << ": " << matchCount << endl;
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%" << endl;
    
    return percentage;
}

void tokenizeReviewText(const string& text, WordFrequency*& wordFreqHead) {
    stringstream ss(text);
    string word;
    
    while (ss >> word) {
        // Clean the word - remove punctuation and convert to lowercase
        string cleanedWord = cleanWord(word);
        
        // Skip empty words or single-character words
        if (cleanedWord.length() > 1) {
            incrementWordFrequency(wordFreqHead, cleanedWord);
        }
    }
}

WordFrequency* analyzeOneStarReviews(Review* head) {
    WordFrequency* wordFreqHead = nullptr;
    int oneStarCount = 0;
    
    Review* current = head;
    while (current) {
        if (current->rating == 1) {
            oneStarCount++;
            tokenizeReviewText(current->reviewText, wordFreqHead);
        }
        current = current->next;
    }
    
    cout << "Found " << oneStarCount << " 1-star reviews" << endl;
    return wordFreqHead;
}

void sortWordFrequency(WordFrequency*& head) {
    if (!head || !head->next) return;
    
    WordFrequency* sorted = nullptr;
    WordFrequency* current = head;
    
    while (current) {
        WordFrequency* next = current->next;
        
        // Insert current in sorted order (by frequency, descending)
        if (!sorted || current->frequency > sorted->frequency) {
            current->next = sorted;
            sorted = current;
        } else {
            WordFrequency* search = sorted;
            while (search->next && search->next->frequency >= current->frequency) {
                search = search->next;
            }
            current->next = search->next;
            search->next = current;
        }
        
        current = next;
    }
    
    head = sorted;
}

void displayWordFrequency(WordFrequency* head, int limit) {
    cout << left << setw(5) << "No" << setw(15) << "Word" << setw(10) << "Count" << endl;
    cout << string(30, '-') << endl;
    
    WordFrequency* current = head;
    int rank = 1;
    
    while (current && rank <= limit) {
        cout << left << setw(5) << rank++ << setw(15) << current->word << setw(10) << current->frequency << endl;
        current = current->next;
    }
}

void displayTransactions(Transaction* head, int limit) {
    Transaction* current = head;
    int count = 0;
    
    cout << "Customer ID,Product,Category,Price,Date,Payment Method" << endl;
    cout << string(70, '-') << endl;
    
    while (current && (limit == -1 || count < limit)) {
        cout << current->customerID << "," 
             << current->product << "," 
             << current->category << "," 
             << fixed << setprecision(2) << current->price << "," 
             << current->date << "," 
             << current->paymentMethod << endl;
        
        current = current->next;
        count++;
    }
    
    if (limit != -1 && count >= limit) {
        cout << "... (showing " << limit << " of " << countTransactions(head) << " transactions)" << endl;
    }
}

void displayTransactionsByDate(DateCountNode* head) {
    DateCountNode* current = head;
    int totalCount = 0;
    int index = 1;
    
    cout << left << setw(5) << "No" << setw(15) << "Date" << setw(10) << "Count" << endl;
    cout << string(30, '-') << endl;
    
    while (current) {
        cout << left << setw(5) << index++ << setw(15) << current->date << setw(10) << current->count << endl;
        totalCount += current->count;
        current = current->next;
    }
    
    cout << string(30, '-') << endl;
    cout << left << setw(20) << "Total" << setw(10) << totalCount << endl;
}

int countTransactions(Transaction* head) {
    int count = 0;
    Transaction* current = head;
    
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
}

int countReviews(Review* head) {
    int count = 0;
    Review* current = head;
    
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
}

// ------------------- Primary Analysis Functions -------------------
void analyzeTransactionsSortedByDate(Transaction* transactionHead, Review* reviewHead, int sortAlgorithm) {
    auto start = high_resolution_clock::now();
    
    cout << "\n----- Sorting Transactions by Date from Customers with Reviews -----\n" << endl;
    
    // Use a linked list instead of a set for customer IDs
    CustomerIDNode* customerIDsHead = nullptr;
    Review* currentReview = reviewHead;
    int reviewerCount = 0;
    
    cout << "Building list of customers who have left reviews..." << endl;
    while (currentReview) {
        insertCustomerID(customerIDsHead, currentReview->customerID);
        currentReview = currentReview->next;
        reviewerCount++;
        
        if (reviewerCount % 1000 == 0) {
            cout << "Processed " << reviewerCount << " reviews\r";
            cout.flush();
        }
    }
    
    // Count unique customers (this will be less efficient than set.size())
    int uniqueCustomers = 0;
    CustomerIDNode* countCurrent = customerIDsHead;
    while (countCurrent) {
        uniqueCustomers++;
        countCurrent = countCurrent->next;
    }
    cout << "\nFound " << uniqueCustomers << " unique customers with reviews" << endl;
    
    // Create a filtered list of transactions from customers with reviews
    Transaction* filteredTransactions = nullptr;
    Transaction* filteredTail = nullptr; // Track the tail for O(1) insertion
    Transaction* current = transactionHead;
    int totalTransactions = 0;
    int filteredCount = 0;
    
    cout << "Filtering transactions for customers with reviews..." << endl;
    while (current) {
        totalTransactions++;
        // Use isCustomerInList instead of set.find()
        if (isCustomerInList(customerIDsHead, current->customerID)) {
            // Create a new node
            Transaction* newNode = new Transaction(*current);
            
            // Add to filtered list with O(1) insertion using tail pointer
            if (filteredTransactions == nullptr) {
                filteredTransactions = newNode;
                filteredTail = newNode;
            } else {
                filteredTail->next = newNode;
                filteredTail = newNode;
            }
            filteredCount++;
        }
        current = current->next;
        
        if (totalTransactions % 1000 == 0) {
            cout << "Processed " << totalTransactions << " transactions, filtered " 
                 << filteredCount << "\r";
            cout.flush();
        }
    }
    cout << "\nFiltered " << filteredCount << " transactions from " << totalTransactions 
         << " total transactions" << endl;
    
    // Free customer IDs list as we don't need it anymore
    freeCustomerIDList(customerIDsHead);
    
    // Sort the filtered transactions by date using selected algorithm
    if (filteredCount > 0) {
        cout << "Beginning sort operation with " << filteredCount << " transactions" << endl;
        
        switch (sortAlgorithm) {
            case 1: {
                cout << "Using Merge Sort... (please wait)" << endl;
                filteredTransactions = mergeSort(filteredTransactions);
                cout << "Merge sort completed" << endl;
                break;
            }
            case 2: {
                cout << "Using Bubble Sort... (may be very slow for large datasets)" << endl;
                if (filteredCount > 10000) {
                    cout << "WARNING: Bubble Sort is inefficient for " << filteredCount << " records." << endl;
                    cout << "This could take a very long time. Continue? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    if (tolower(confirm) != 'y') {
                        cout << "Operation canceled." << endl;
                        freeTransactionList(filteredTransactions);
                        return;
                    }
                }
                bubbleSort(filteredTransactions);
                cout << "Bubble sort completed" << endl;
                break;
            }
            case 3: {
                cout << "Using Insertion Sort... (please wait)" << endl;
                insertionSort(filteredTransactions);
                cout << "Insertion sort completed" << endl;
                break;
            }
            case 4: {
                cout << "Using Quick Sort... (please wait)" << endl;
                if (filteredCount > 0) {
                    Transaction* last = getLastNode(filteredTransactions);
                    improvedQuickSort(filteredTransactions, last);
                    cout << "\nQuick sort completed                " << endl;
                }
                break;
            }
            default:
                cout << "Invalid choice. Using Quick Sort by default." << endl;
                if (filteredCount > 0) {
                    Transaction* last = getLastNode(filteredTransactions);
                    improvedQuickSort(filteredTransactions, last);
                    cout << "\nQuick sort completed                " << endl;
                }
        }
        
        // Display results
        cout << "\nDisplaying first 10 transactions after filtering and sorting:" << endl;
        displayTransactions(filteredTransactions, 10);
        
        // Calculate transaction counts by date
        DateCountNode* dateCountHead = countTransactionsByDate(filteredTransactions);
        cout << "\nTransaction counts by date for customers with reviews:" << endl;
        displayTransactionsByDate(dateCountHead);
        
        // Free memory for date counts
        freeDateCountList(dateCountHead);
        
        // Free filtered transactions
        freeTransactionList(filteredTransactions);
    } else {
        cout << "No transactions found for customers with reviews." << endl;
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "\nTime taken: " << duration << " ms" << endl;
}

void analyzeElectronicsCreditCardPayments(Transaction* transactionHead, int searchAlgorithm) {
    auto start = high_resolution_clock::now();
    
    cout << "\n----- Electronics Category Credit Card Payments -----\n" << endl;
    cout << "Debug: Beginning payment analysis with " << countTransactions(transactionHead) << " transactions" << endl;
    
    // Calculate percentage using selected search algorithm
    cout << "Debug: Starting search algorithm " << searchAlgorithm << endl;
    calculateCategoryPaymentPercentage(transactionHead, "Electronics", "Credit Card", searchAlgorithm);
    cout << "Debug: Search algorithm completed" << endl;
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    cout << "\nExecution time: " << duration.count() << " milliseconds" << endl;
}

void analyzeFrequentWordsInLowRatedReviews(Review* reviewHead) {
    auto start = high_resolution_clock::now();
    
    cout << "\n----- Most Frequent Words in 1-Star Reviews -----\n" << endl;
    
    // Find frequent words in 1-star reviews
    WordFrequency* frequentWords = analyzeOneStarReviews(reviewHead);
    
    // Sort by frequency
    sortWordFrequency(frequentWords);
    
    // Display top words
    cout << "\nTop 10 most frequent words in 1-star reviews:" << endl;
    displayWordFrequency(frequentWords, 10);
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    cout << "\nExecution time: " << duration.count() << " milliseconds" << endl;
    
    // Clean up
    freeWordFrequencyList(frequentWords);
}

// ------------------- Main Function -------------------
int main() {
    cout << "\n===================================================\n";
    cout << "    E-COMMERCE DATA ANALYSIS USING LINKED LISTS    \n";
    cout << "===================================================\n";
    
    int transactionCount = 0;
    int reviewCount = 0;
    
    // Load transaction data
    cout << "\nLoading transaction data..." << endl;
    Transaction* transactionHead = loadTransactionsFromCSV("transactions_cleaned.csv", transactionCount);
    if (!transactionHead) {
        cerr << "Failed to load transaction data. Exiting." << endl;
        return 1;
    }
    cout << "Successfully loaded " << transactionCount << " transactions." << endl;
    
    // Load review data
    cout << "\nLoading review data..." << endl;
    Review* reviewHead = loadReviewsFromCSV("reviews_cleaned.csv", reviewCount);
    if (!reviewHead) {
        cerr << "Failed to load review data. Exiting." << endl;
        freeTransactionList(transactionHead);
        return 1;
    }
    cout << "Successfully loaded " << reviewCount << " reviews." << endl;
    
    int choice = 0;
    int sortChoice = 0;
    int searchChoice = 0;
    
    do {
        // Display menu
        cout << "\n===================================================\n";
        cout << "                      MENU                         \n";
        cout << "===================================================\n";
        cout << "1. Sort Transactions by Date from Customers with Reviews\n"; // Updated option
        cout << "2. Calculate Electronics Credit Card Payment Percentage\n";
        cout << "3. Analyze Word Frequency in 1-Star Reviews\n";
        cout << "4. Run All Analyses\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\nChoose a sorting algorithm:\n";
                cout << "1. Merge Sort\n";
                cout << "2. Bubble Sort\n";
                cout << "3. Insertion Sort\n";
                cout << "4. Quick Sort\n";
                cout << "Enter your choice: ";
                cin >> sortChoice;
                
                cout << "Debug: Creating transaction copy for sorting..." << endl;
                // Create a copy of transactionHead for sorting - OPTIMIZED VERSION
                Transaction* transactionCopy = nullptr;
                Transaction* tail = nullptr;  // Keep track of the tail
                Transaction* current = transactionHead;
                int copyCount = 0;
                
                while (current) {
                    // Create a new node directly
                    Transaction* newNode = new Transaction(*current);
                    
                    if (transactionCopy == nullptr) {
                        transactionCopy = newNode;
                        tail = newNode;
                    } else {
                        tail->next = newNode;
                        tail = newNode;
                    }
                    
                    current = current->next;
                    copyCount++;
                    
                    if (copyCount % 1000 == 0) {
                        cout << "Debug: Copied " << copyCount << " transactions\r";
                        cout.flush();
                    }
                }
                cout << "\nDebug: Copy completed with " << copyCount << " transactions" << endl;
                
                analyzeTransactionsSortedByDate(transactionCopy, reviewHead, sortChoice);
                
                // Clean up
                cout << "Debug: Freeing transaction copy..." << endl;
                freeTransactionList(transactionCopy);
                cout << "Debug: Memory freed successfully" << endl;
                break;
            }
            case 2: {
                cout << "\nChoose a searching algorithm:\n";
                cout << "1. Linear Search\n";
                cout << "2. Binary Search\n";
                cout << "3. Jump Search\n";
                cout << "4. Exponential Search\n";
                cout << "Enter your choice: ";
                cin >> searchChoice;
                
                analyzeElectronicsCreditCardPayments(transactionHead, searchChoice);
                break;
            }
            case 3: {
                analyzeFrequentWordsInLowRatedReviews(reviewHead);
                break;
            }
            case 4: {
                cout << "\n===== Run All Analyses =====\n" << endl;
    
                int sortOption = 0;
                int searchOption = 0;
                
                cout << "Choose a sorting algorithm for the first analysis:" << endl;
                cout << "1 - Bubble Sort" << endl;
                cout << "2 - Quick Sort" << endl;
                cout << "3 - Insertion Sort" << endl;
                cout << "4 - Merge Sort (Default)" << endl;
                cout << "Enter choice (1-4): ";
                cin >> sortOption;
                
                if (sortOption < 1 || sortOption > 4) {
                    cout << "Invalid choice. Using Merge Sort as default." << endl;
                    sortOption = 4; // Default to Merge Sort
                }
                
                cout << "\nChoose a searching algorithm for the second analysis:" << endl;
                cout << "1 - Linear Search (Default)" << endl;
                cout << "2 - Binary Search" << endl;
                cout << "3 - Jump Search" << endl;
                cout << "4 - Exponential Search" << endl;
                cout << "Enter choice (1-4): ";
                cin >> searchOption;
                
                if (searchOption < 1 || searchOption > 4) {
                    cout << "Invalid choice. Using Linear Search as default." << endl;
                    searchOption = 1; // Default to Linear Search
                }
                
                cout << "\nRunning all analyses with selected algorithms..." << endl;

                // Analysis 1: Transactions by Date (with selected sorting algorithm)
                cout << "Debug: Creating transaction copy for first analysis..." << endl;
                Transaction* transactionCopy = nullptr;
                Transaction* tail = nullptr;  // Keep track of the tail
                Transaction* current = transactionHead;
                int copyCount = 0;

                while (current) {
                    // Create a new node directly
                    Transaction* newNode = new Transaction(*current);
                    
                    if (transactionCopy == nullptr) {
                        transactionCopy = newNode;
                        tail = newNode;
                    } else {
                        tail->next = newNode;
                        tail = newNode;
                    }
                    
                    current = current->next;
                    copyCount++;
                    
                    if (copyCount % 1000 == 0) {
                        cout << "Debug: Copied " << copyCount << " transactions\r";
                        cout.flush();
                    }
                }
                cout << "\nDebug: Copy completed with " << copyCount << " transactions" << endl;

                // Run first analysis with selected sorting algorithm
                analyzeTransactionsSortedByDate(transactionCopy, reviewHead, sortOption);
                
                // Analysis 2: Electronics Credit Card Payments (with selected search algorithm)
                analyzeElectronicsCreditCardPayments(transactionHead, searchOption);
                
                // Analysis 3: One-Star Reviews Analysis
                analyzeFrequentWordsInLowRatedReviews(reviewHead);
                
                break;
            }
            case 0:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
    
    // Clean up
    freeTransactionList(transactionHead);
    freeReviewList(reviewHead);
    
    return 0;
}