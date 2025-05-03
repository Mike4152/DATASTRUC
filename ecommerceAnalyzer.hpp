#ifndef ECOMMERCE_ANALYZER_HPP
#define ECOMMERCE_ANALYZER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <cctype>

// Unified Transaction structure for all algorithms
struct Transaction {
    std::string customerID;
    std::string product;
    std::string category;
    double price;
    std::string date;
    std::string paymentMethod;
    Transaction* next;
    
    Transaction();
    Transaction(const std::string& cid, const std::string& prod, const std::string& cat, 
                double pr, const std::string& dt, const std::string& pm);
};

struct CustomerIDNode {
    std::string customerID;
    CustomerIDNode* next;
    
    CustomerIDNode(const std::string& id) : customerID(id), next(nullptr) {}
};

// Unified Review structure
struct Review {
    std::string productID;
    std::string customerID;
    int rating;
    std::string reviewText;
    Review* next;
    
    Review();
    Review(const std::string& pid, const std::string& cid, int r, const std::string& text);
};

// Structure for date-count pairs
struct DateCountNode {
    std::string date;
    int count;
    DateCountNode* next;
    
    DateCountNode(const std::string& d = "", int c = 0);
};

// Structure for word frequency
struct WordFrequency {
    std::string word;
    int frequency;
    WordFrequency* next;
    
    WordFrequency(const std::string& w = "", int f = 0);
};

// Utility functions
std::string trim(const std::string& str);
std::string convertDateFormat(const std::string& date);
bool isDateLess(const std::string& date1, const std::string& date2);
std::string cleanWord(const std::string& word);

// Linked list operations
void insertTransaction(Transaction*& head, const Transaction& t);
void insertTransaction(Transaction*& head, const std::string& customerID, const std::string& product, 
                      const std::string& category, double price, const std::string& date, 
                      const std::string& paymentMethod);
void insertReview(Review*& head, const std::string& productID, const std::string& customerID, 
                 int rating, const std::string& reviewText);
void insertDateCount(DateCountNode*& head, const std::string& date);
void insertWordFrequency(WordFrequency*& head, const std::string& word, int frequency);
void incrementWordFrequency(WordFrequency*& head, const std::string& word);

// Memory management
void freeTransactionList(Transaction*& head);
void freeReviewList(Review*& head);
void freeDateCountList(DateCountNode*& head);
void freeWordFrequencyList(WordFrequency*& head);

// File operations
Transaction* loadTransactionsFromCSV(const std::string& filename, int& count);
Review* loadReviewsFromCSV(const std::string& filename, int& count);

// Sorting algorithms
// 1. Merge Sort (from wendyLinkedList.cpp)
Transaction* mergeSort(Transaction* head);
Transaction* merge(Transaction* left, Transaction* right);
Transaction* findMiddle(Transaction* head);

// 2. Bubble Sort (from mahmoudLinkedList.cpp)
void bubbleSort(Transaction*& head);

// 3. Insertion Sort (from wendyLinkedList.cpp)
void insertionSort(Transaction*& head);

// 4. Quick Sort (from devaraLinkedList.cpp)
void quickSort(Transaction* start, Transaction* end);
Transaction* getLastNode(Transaction* head);
Transaction* partition(Transaction* start, Transaction* end);
void swapTransactionData(Transaction* a, Transaction* b);

// Searching algorithms
// 1. Linear Search
int linearSearch(Transaction* head, const std::string& category, const std::string& paymentMethod, 
                int& totalCount, int& matchCount);

// 2. Binary Search
int binarySearch(Transaction* head, const std::string& category, const std::string& paymentMethod,
                int& totalCount, int& matchCount);

// 3. Jump Search
int jumpSearch(Transaction* head, const std::string& category, const std::string& paymentMethod,
              int& totalCount, int& matchCount);

// 4. Exponential Search
int exponentialSearch(Transaction* head, const std::string& category, const std::string& paymentMethod,
                     int& totalCount, int& matchCount);

// Analysis functions
DateCountNode* countTransactionsByDate(Transaction* head);
double calculateCategoryPaymentPercentage(Transaction* head, const std::string& category, 
                                        const std::string& paymentMethod, int algorithm);
WordFrequency* analyzeOneStarReviews(Review* head);
void sortWordFrequency(WordFrequency*& head);
void displayWordFrequency(WordFrequency* head, int limit = 10);
void tokenizeReviewText(const std::string& text, WordFrequency*& wordFreqHead);

// Display functions
void displayTransactions(Transaction* head, int limit = 10);
void displayTransactionsByDate(DateCountNode* head);
int countTransactions(Transaction* head);
int countReviews(Review* head);

// Primary analysis functions
void analyzeTransactionsSortedByDate(Transaction* transactionHead, Review* reviewHead, int sortAlgorithm);
void analyzeElectronicsCreditCardPayments(Transaction* transactionHead, int searchAlgorithm);
void analyzeFrequentWordsInLowRatedReviews(Review* reviewHead);

#endif // ECOMMERCE_ANALYZER_HPP