#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <set>
#include <cctype>

using namespace std;
using namespace chrono;

// Constants
const int MAX_TRANSACTIONS = 10000;
const int MAX_DATES = 1000;

// ==== COMMON STRUCTURES ====
struct Transaction {
    string customer_id;
    string product_id;
    string category;
    string payment_method;
    string date;
};

struct DateCount {
    string date;
    int count;
};

struct WordFreq {
    string word;
    int count;
};

// ==== COMMON UTILS ====
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string convertDate(const string& date) {
    if (date.size() != 10) return date;
    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    string year = date.substr(6, 4);
    return year + "-" + month + "-" + day;
}

string cleanWord(const string& word) {
    string cleaned;
    for (char ch : word) {
        if (isalpha(ch))
            cleaned += tolower(ch);
    }
    return cleaned;
}

// ==== Question 1 ====
void question1() {
    auto totalStart = high_resolution_clock::now();

    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount = 0;
    DateCount dateCounts[MAX_DATES];
    int dateCount = 0;
    set<string> reviewCustomers;

    string line;

    // Transactions
    auto transactionStart = high_resolution_clock::now();
    ifstream transactionFile("transactions_cleaned.csv");
    if (!transactionFile.is_open()) {
        cerr << "Error opening transactions file!" << endl;
        return;
    }
    getline(transactionFile, line); // Skip header

    while (getline(transactionFile, line) && transactionCount < MAX_TRANSACTIONS) {
        stringstream ss(line);
        string customer_id, product_name, category, price, date;

        getline(ss, customer_id, ',');
        getline(ss, product_name, ',');
        getline(ss, category, ',');
        getline(ss, price, ',');
        getline(ss, date, ',');

        customer_id = trim(customer_id);
        date = trim(date);
        date = convertDate(date);

        transactions[transactionCount++] = {customer_id, "", category, "", date};
    }
    transactionFile.close();
    auto transactionEnd = high_resolution_clock::now();

    // Reviews
    auto reviewStart = high_resolution_clock::now();
    ifstream reviewFile("reviews_cleaned.csv");
    if (!reviewFile.is_open()) {
        cerr << "Error opening reviews file!" << endl;
        return;
    }
    getline(reviewFile, line); // Skip header
    while (getline(reviewFile, line)) {
        stringstream ss(line);
        string skipField, customer_id;
        getline(ss, skipField, ',');
        getline(ss, customer_id, ',');
        reviewCustomers.insert(trim(customer_id));
    }
    reviewFile.close();
    auto reviewEnd = high_resolution_clock::now();

    // Count per date
    for (int i = 0; i < transactionCount; ++i) {
        if (reviewCustomers.find(transactions[i].customer_id) != reviewCustomers.end()) {
            bool found = false;
            for (int j = 0; j < dateCount; ++j) {
                if (dateCounts[j].date == transactions[i].date) {
                    dateCounts[j].count++;
                    found = true;
                    break;
                }
            }
            if (!found && dateCount < MAX_DATES) {
                dateCounts[dateCount++] = {transactions[i].date, 1};
            }
        }
    }

    // Sort by date (Insertion Sort)
    auto sortStart = high_resolution_clock::now();
    for (int i = 1; i < dateCount; ++i) {
        DateCount key = dateCounts[i];
        int j = i - 1;
        while (j >= 0 && dateCounts[j].date > key.date) {
            dateCounts[j + 1] = dateCounts[j];
            j--;
        }
        dateCounts[j + 1] = key;
    }
    auto sortEnd = high_resolution_clock::now();

    // Output
    int total = 0;
    cout << "\n=== Transaction Counts by Date ===" << endl;
    for (int i = 0; i < dateCount; ++i) {
        cout << setw(3) << i + 1 << ") " << setw(12) << dateCounts[i].date << " " << setw(5) << dateCounts[i].count << endl;
        total += dateCounts[i].count;
    }
    cout << "---------------------------------------------" << endl;
    cout << setw(15) << "Total" << setw(10) << total << endl;

    // Time reports
    cout << fixed << setprecision(3);
    cout << "\n=== Timing Results ===" << endl;
    cout << "Transactions Read Time: " << duration<double, milli>(transactionEnd - transactionStart).count() << " ms" << endl;
    cout << "Reviews Read Time: " << duration<double, milli>(reviewEnd - reviewStart).count() << " ms" << endl;
    cout << "Insertion Sort Time: " << duration<double, milli>(sortEnd - sortStart).count() << " ms" << endl;
    cout << "Total Execution Time: " << duration<double, milli>(high_resolution_clock::now() - totalStart).count() << " ms" << endl;
}

// ==== Question 2 ====
void question2() {
    auto start = high_resolution_clock::now();

    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount = 0;
    string paymentMethods[MAX_TRANSACTIONS];
    int electronicsCount = 0;

    ifstream file("transactions_cleaned.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header
    while (getline(file, line) && transactionCount < MAX_TRANSACTIONS) {
        stringstream ss(line);
        string fields[10];
        int col = 0;
        while (getline(ss, fields[col], ',') && col < 10) {
            fields[col] = trim(fields[col]);
            col++;
        }
        transactions[transactionCount++] = {
            fields[0], fields[1], fields[2], fields[5], fields[4]
        };
    }
    file.close();

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].category == "Electronics") {
            paymentMethods[electronicsCount++] = transactions[i].payment_method;
        }
    }

    // Insertion Sort
    for (int i = 1; i < electronicsCount; i++) {
        string key = paymentMethods[i];
        int j = i - 1;
        while (j >= 0 && paymentMethods[j] > key) {
            paymentMethods[j + 1] = paymentMethods[j];
            j--;
        }
        paymentMethods[j + 1] = key;
    }

    // Jump Search
    int step = sqrt(electronicsCount);
    int prev = 0, found = -1;
    while (paymentMethods[min(step, electronicsCount) - 1] < "Credit Card") {
        prev = step;
        step += sqrt(electronicsCount);
        if (prev >= electronicsCount) break;
    }
    for (int i = prev; i < min(step, electronicsCount); i++) {
        if (paymentMethods[i] == "Credit Card") {
            found = i;
            break;
        }
    }

    int creditCardCount = 0;
    if (found != -1) {
        int left = found;
        while (left >= 0 && paymentMethods[left] == "Credit Card") {
            creditCardCount++;
            left--;
        }
        int right = found + 1;
        while (right < electronicsCount && paymentMethods[right] == "Credit Card") {
            creditCardCount++;
            right++;
        }
    }

    double percentage = (electronicsCount == 0) ? 0 : (double)creditCardCount / electronicsCount * 100.0;
    cout << "\n=== Credit Card Usage in Electronics ===" << endl;
    cout << "Total Electronics: " << electronicsCount << endl;
    cout << "Credit Card Used: " << creditCardCount << endl;
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%" << endl;

    auto end = high_resolution_clock::now();
    cout << "[Execution Time] Total: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}

// ==== Question 3 ====
void question3() {
    auto start = high_resolution_clock::now();

    ifstream file("reviews_cleaned.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    WordFreq words[10000];
    int size = 0;
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id, customerID, ratingStr, reviewText;

        getline(ss, id, ',');
        getline(ss, customerID, ',');
        getline(ss, ratingStr, ',');
        getline(ss, reviewText);

        int rating = 0;
        try { rating = stoi(ratingStr); } catch (...) { continue; }
        if (rating == 1) {
            stringstream wordsStream(reviewText);
            string word;
            while (wordsStream >> word) {
                word = cleanWord(word);
                if (!word.empty()) {
                    bool found = false;
                    for (int i = 0; i < size; ++i) {
                        if (words[i].word == word) {
                            words[i].count++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        words[size++] = {word, 1};
                    }
                }
            }
        }
    }
    file.close();

    // Sort by count
    for (int i = 1; i < size; ++i) {
        WordFreq key = words[i];
        int j = i - 1;
        while (j >= 0 && words[j].count < key.count) {
            words[j + 1] = words[j];
            j--;
        }
        words[j + 1] = key;
    }

    cout << "\n=== Top 10 Words in 1-Star Reviews ===" << endl;
    cout << left << setw(5) << "No" << setw(15) << "Word" << setw(10) << "Count" << endl;
    cout << "----------------------------------" << endl;
    for (int i = 0; i < 10 && i < size; ++i) {
        cout << left << setw(5) << i + 1 << setw(15) << words[i].word << setw(10) << words[i].count << endl;
    }

    auto end = high_resolution_clock::now();
    cout << "\n[Timing Report] Total execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
}

// ==== MAIN MENU ====
int main() {

   
    question1() ; 
    question2();
    question3();

    return 0;
   
}
