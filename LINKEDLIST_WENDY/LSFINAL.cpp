#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <cmath>
#include <cctype>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ------------------- Shared Utility -------------------
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string convertDate(const string& date) {
    if (date.size() != 10) return date;
    return date.substr(6, 4) + "-" + date.substr(3, 2) + "-" + date.substr(0, 2);
}

// ------------------- Question 1 -------------------
struct TransactionNode {
    string customer_id, date;
    TransactionNode* next;
};

struct DateCountNode {
    string date;
    int count;
    DateCountNode* next;
};

TransactionNode* createTransactionNode(string customer_id, string date) {
    return new TransactionNode{customer_id, date, nullptr};
}

void insertTransactionNode(TransactionNode*& head, string customer_id, string date) {
    TransactionNode* newNode = createTransactionNode(customer_id, date);
    if (!head) head = newNode;
    else {
        TransactionNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

DateCountNode* createDateCountNode(string date) {
    return new DateCountNode{date, 1, nullptr};
}

void insertOrUpdateDateCount(DateCountNode*& head, string date) {
    DateCountNode* temp = head;
    while (temp) {
        if (temp->date == date) {
            temp->count++;
            return;
        }
        temp = temp->next;
    }
    DateCountNode* newNode = createDateCountNode(date);
    if (!head) head = newNode;
    else {
        temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

void insertionSortDateCounts(DateCountNode*& head) {
    DateCountNode* sorted = nullptr;
    while (head) {
        DateCountNode* current = head;
        head = head->next;
        if (!sorted || current->date < sorted->date) {
            current->next = sorted;
            sorted = current;
        } else {
            DateCountNode* temp = sorted;
            while (temp->next && temp->next->date < current->date) temp = temp->next;
            current->next = temp->next;
            temp->next = current;
        }
    }
    head = sorted;
}

void question1() {
    auto start = high_resolution_clock::now();
    TransactionNode* transactionHead = nullptr;
    DateCountNode* dateCountHead = nullptr;
    set<string> reviewCustomers;

    ifstream transactionFile("transactions_cleaned.csv");
    if (!transactionFile) {
        cerr << "Cannot open transactions file.\n"; return;
    }
    string line;
    getline(transactionFile, line);
    while (getline(transactionFile, line)) {
        stringstream ss(line);
        string customer_id, _, __, ___, date;
        getline(ss, customer_id, ',');
        for (int i = 0; i < 3; i++) getline(ss, _, ',');
        getline(ss, date, ',');
        insertTransactionNode(transactionHead, trim(customer_id), convertDate(trim(date)));
    }
    transactionFile.close();

    ifstream reviewFile("reviews_cleaned.csv");
    if (!reviewFile) {
        cerr << "Cannot open reviews file.\n"; return;
    }
    getline(reviewFile, line);
    while (getline(reviewFile, line)) {
        stringstream ss(line);
        string _, customer_id;
        getline(ss, _, ',');
        getline(ss, customer_id, ',');
        reviewCustomers.insert(trim(customer_id));
    }
    reviewFile.close();

    for (TransactionNode* t = transactionHead; t; t = t->next) {
        if (reviewCustomers.count(t->customer_id)) insertOrUpdateDateCount(dateCountHead, t->date);
    }
    insertionSortDateCounts(dateCountHead);

    int serial = 1, total = 0;
    for (DateCountNode* cur = dateCountHead; cur; cur = cur->next) {
        cout << serial++ << ") " << setw(12) << cur->date << " " << setw(5) << cur->count << endl;
        total += cur->count;
    }
    cout << "---------------------------------------------\n";
    cout << setw(15) << "Total" << setw(10) << total << endl;

    auto end = high_resolution_clock::now();
    cout << "Execution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

// ------------------- Question 2 -------------------
struct FullTransactionNode {
    string customer_id, product_id, category, payment_method, date;
    FullTransactionNode* next;
};

void insertFullTransaction(FullTransactionNode*& head, FullTransactionNode* newNode) {
    if (!head) head = newNode;
    else {
        FullTransactionNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

FullTransactionNode* filterElectronics(FullTransactionNode* head, int& count) {
    FullTransactionNode* result = nullptr;
    count = 0;
    while (head) {
        if (head->category == "Electronics") {
            insertFullTransaction(result, new FullTransactionNode{"", "", "", head->payment_method, "", nullptr});
            count++;
        }
        head = head->next;
    }
    return result;
}

FullTransactionNode* sortByPaymentMethod(FullTransactionNode* head) {
    FullTransactionNode* sorted = nullptr;
    while (head) {
        FullTransactionNode* cur = head;
        head = head->next;
        if (!sorted || cur->payment_method < sorted->payment_method) {
            cur->next = sorted;
            sorted = cur;
        } else {
            FullTransactionNode* temp = sorted;
            while (temp->next && temp->next->payment_method < cur->payment_method) temp = temp->next;
            cur->next = temp->next;
            temp->next = cur;
        }
    }
    return sorted;
}

int countMethod(FullTransactionNode* head, string method) {
    int count = 0;
    while (head) {
        if (head->payment_method == method) count++;
        head = head->next;
    }
    return count;
}

void question2() {
    auto start = high_resolution_clock::now();
    FullTransactionNode* head = nullptr;
    ifstream file("transactions_cleaned.csv");
    if (!file) {
        cerr << "Cannot open file.\n"; return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string fields[10];
        for (int i = 0; i < 10; ++i) getline(ss, fields[i], ',');
        insertFullTransaction(head, new FullTransactionNode{
            trim(fields[0]), trim(fields[1]), trim(fields[2]), trim(fields[5]), trim(fields[4]), nullptr});
    }
    file.close();

    int electronicsCount = 0;
    FullTransactionNode* electronics = filterElectronics(head, electronicsCount);
    electronics = sortByPaymentMethod(electronics);

    int creditCount = countMethod(electronics, "Credit Card");
    double percentage = electronicsCount ? (double)creditCount / electronicsCount * 100 : 0;

    cout << "Total Electronics purchases: " << electronicsCount << endl;
    cout << "Credit Card payments: " << creditCount << endl;
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";

    auto end = high_resolution_clock::now();
    cout << "Execution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

// ------------------- Question 3 -------------------
struct WordNode {
    string word;
    int count;
    WordNode* next;
};

string cleanWord(const string& word) {
    string result;
    for (char ch : word)
        if (isalpha(ch)) result += tolower(ch);
    return result;
}

void insertWord(WordNode*& head, const string& word) {
    for (WordNode* cur = head; cur; cur = cur->next) {
        if (cur->word == word) {
            cur->count++;
            return;
        }
    }
    head = new WordNode{word, 1, head};
}

void sortWords(WordNode*& head) {
    WordNode* sorted = nullptr;
    while (head) {
        WordNode* cur = head;
        head = head->next;
        if (!sorted || cur->count > sorted->count) {
            cur->next = sorted;
            sorted = cur;
        } else {
            WordNode* temp = sorted;
            while (temp->next && temp->next->count >= cur->count) temp = temp->next;
            cur->next = temp->next;
            temp->next = cur;
        }
    }
    head = sorted;
}

void question3() {
    auto start = high_resolution_clock::now();
    ifstream file("reviews_cleaned.csv");
    if (!file) {
        cerr << "Cannot open reviews file.\n"; return;
    }
    string line;
    getline(file, line);
    WordNode* head = nullptr;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, cust_id, ratingStr, review;
        getline(ss, id, ','); getline(ss, cust_id, ','); getline(ss, ratingStr, ','); getline(ss, review);

        try {
            if (stoi(ratingStr) == 1) {
                stringstream wss(review);
                string word;
                while (wss >> word) {
                    word = cleanWord(word);
                    if (!word.empty()) insertWord(head, word);
                }
            }
        } catch (...) {}
    }
    file.close();
    sortWords(head);

    cout << left << setw(5) << "No" << setw(15) << "Word" << setw(10) << "Count" << endl;
    cout << "---------------------------------\n";
    int rank = 1;
    for (WordNode* cur = head; cur && rank <= 10; cur = cur->next)
        cout << left << setw(5) << rank++ << setw(15) << cur->word << setw(10) << cur->count << endl;

    auto end = high_resolution_clock::now();
    cout << "Execution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

// ------------------- Main Menu -------------------
int main() {
    question1();
    question2();
    question3();

    return 0;
}
