/*
 * Library Management System - Backend
 * Object-Oriented Programming Project
 * C++ HTTP Server with Library Classes
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

// Base class for all library resources
class LibraryResource {
protected:
    int resourceID;
    string title;
    string author;
    bool isAvailable;

public:
    LibraryResource(int id = 0, string t = "", string a = "", bool avail = true) {
        resourceID = id;
        title = t;
        author = a;
        isAvailable = avail;
    }

    // Setters
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setAvailability(bool avail) { isAvailable = avail; }

    // Getters
    int getID() const { return resourceID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getAvailability() const { return isAvailable; }

    // Virtual functions for polymorphism
    virtual string getType() const { return "Resource"; }
    virtual string getExtraInfo() const { return ""; }
    virtual int calculateLateFee(int daysLate) const { return 0; }

    // Convert to JSON string
    virtual string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"id\":" << resourceID << ",";
        ss << "\"type\":\"" << getType() << "\",";
        ss << "\"title\":\"" << title << "\",";
        ss << "\"author\":\"" << author << "\",";
        ss << "\"available\":" << (isAvailable ? "true" : "false") << ",";
        ss << "\"extra\":\"" << getExtraInfo() << "\"";
        ss << "}";
        return ss.str();
    }

    virtual ~LibraryResource() {}
};

// Book class - inherits from LibraryResource
class Book : public LibraryResource {
    string ISBN;
    int pageCount;

public:
    Book(int id = 0, string t = "", string a = "", string isbn = "", int pages = 0)
        : LibraryResource(id, t, a) {
        ISBN = isbn;
        pageCount = pages;
    }

    string getType() const override { return "Book"; }
    string getExtraInfo() const override {
        stringstream ss;
        ss << "ISBN:" << ISBN << "|Pages:" << pageCount;
        return ss.str();
    }

    int calculateLateFee(int daysLate) const override {
        return daysLate * 5; // Rs. 5 per day for books
    }

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"id\":" << resourceID << ",";
        ss << "\"type\":\"Book\",";
        ss << "\"title\":\"" << title << "\",";
        ss << "\"author\":\"" << author << "\",";
        ss << "\"isbn\":\"" << ISBN << "\",";
        ss << "\"pages\":" << pageCount << ",";
        ss << "\"available\":" << (isAvailable ? "true" : "false");
        ss << "}";
        return ss.str();
    }
};

// Magazine class - inherits from LibraryResource
class Magazine : public LibraryResource {
    int issueNumber;

public:
    Magazine(int id = 0, string t = "", string a = "", int issue = 0)
        : LibraryResource(id, t, a) {
        issueNumber = issue;
    }

    string getType() const override { return "Magazine"; }
    string getExtraInfo() const override {
        stringstream ss;
        ss << "Issue:" << issueNumber;
        return ss.str();
    }

    int calculateLateFee(int daysLate) const override {
        return daysLate * 3; // Rs. 3 per day for magazines
    }

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"id\":" << resourceID << ",";
        ss << "\"type\":\"Magazine\",";
        ss << "\"title\":\"" << title << "\",";
        ss << "\"author\":\"" << author << "\",";
        ss << "\"issue\":" << issueNumber << ",";
        ss << "\"available\":" << (isAvailable ? "true" : "false");
        ss << "}";
        return ss.str();
    }
};

// DVD class - inherits from LibraryResource
class DVD : public LibraryResource {
    int duration; // in minutes

public:
    DVD(int id = 0, string t = "", string a = "", int d = 0)
        : LibraryResource(id, t, a) {
        duration = d;
    }

    string getType() const override { return "DVD"; }
    string getExtraInfo() const override {
        stringstream ss;
        ss << "Duration:" << duration << "min";
        return ss.str();
    }

    int calculateLateFee(int daysLate) const override {
        return daysLate * 10; // Rs. 10 per day for DVDs
    }

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"id\":" << resourceID << ",";
        ss << "\"type\":\"DVD\",";
        ss << "\"title\":\"" << title << "\",";
        ss << "\"author\":\"" << author << "\",";
        ss << "\"duration\":" << duration << ",";
        ss << "\"available\":" << (isAvailable ? "true" : "false");
        ss << "}";
        return ss.str();
    }
};

// Library Member class
class LibraryMember {
    int memberID;
    string name;
    vector<int> borrowedResourceIDs; // Store IDs of borrowed resources
    vector<string> borrowDates; // Store borrow dates

public:
    LibraryMember(int id = 0, string n = "") {
        memberID = id;
        name = n;
    }

    int getID() const { return memberID; }
    string getName() const { return name; }

    // Borrow a resource
    bool borrowResource(int resourceID) {
        // Check if already borrowed
        for (int id : borrowedResourceIDs) {
            if (id == resourceID) {
                return false; // Already borrowed
            }
        }
        borrowedResourceIDs.push_back(resourceID);
        
        // Store current date
        time_t now = time(0);
        char* dt = ctime(&now);
        string dateStr(dt);
        dateStr.pop_back(); // Remove newline
        borrowDates.push_back(dateStr);
        
        return true;
    }

    // Return a resource
    bool returnResource(int resourceID) {
        for (size_t i = 0; i < borrowedResourceIDs.size(); i++) {
            if (borrowedResourceIDs[i] == resourceID) {
                borrowedResourceIDs.erase(borrowedResourceIDs.begin() + i);
                borrowDates.erase(borrowDates.begin() + i);
                return true;
            }
        }
        return false;
    }

    // Get borrowed resources count
    int getBorrowedCount() const {
        return borrowedResourceIDs.size();
    }

    // Get borrowed resource IDs
    const vector<int>& getBorrowedIDs() const {
        return borrowedResourceIDs;
    }

    // Get borrow date for a resource
    string getBorrowDate(int resourceID) const {
        for (size_t i = 0; i < borrowedResourceIDs.size(); i++) {
            if (borrowedResourceIDs[i] == resourceID) {
                return borrowDates[i];
            }
        }
        return "";
    }

    // Calculate total late fee
    int calculateTotalLateFee(const vector<LibraryResource*>& allResources, int daysLate) const {
        int total = 0;
        for (int resID : borrowedResourceIDs) {
            for (const auto& res : allResources) {
                if (res->getID() == resID) {
                    total += res->calculateLateFee(daysLate);
                    break;
                }
            }
        }
        return total;
    }

    // Convert to JSON
    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"id\":" << memberID << ",";
        ss << "\"name\":\"" << name << "\",";
        ss << "\"borrowedCount\":" << borrowedResourceIDs.size() << ",";
        ss << "\"borrowedIDs\":[";
        for (size_t i = 0; i < borrowedResourceIDs.size(); i++) {
            ss << borrowedResourceIDs[i];
            if (i < borrowedResourceIDs.size() - 1) ss << ",";
        }
        ss << "]";
        ss << "}";
        return ss.str();
    }
};

// Library class - manages all resources and members
class Library {
    vector<LibraryResource*> resources;
    vector<LibraryMember> members;
    int nextResourceID;
    int nextMemberID;

public:
    Library() {
        nextResourceID = 1;
        nextMemberID = 1;
        loadFromFile();
    }

    ~Library() {
        // Clean up dynamically allocated resources
        for (auto res : resources) {
            delete res;
        }
    }

    // Add a new resource
    int addResource(string type, string title, string author, string extra1, string extra2) {
        int id = nextResourceID++;
        LibraryResource* res = nullptr;

        if (type == "Book") {
            res = new Book(id, title, author, extra1, stoi(extra2));
        } else if (type == "Magazine") {
            res = new Magazine(id, title, author, stoi(extra1));
        } else if (type == "DVD") {
            res = new DVD(id, title, author, stoi(extra1));
        }

        if (res) {
            resources.push_back(res);
            saveToFile();
        }
        return id;
    }

    // Register a new member
    int addMember(string name) {
        int id = nextMemberID++;
        members.push_back(LibraryMember(id, name));
        saveToFile();
        return id;
    }

    // Find resource by ID
    LibraryResource* findResource(int id) {
        for (auto res : resources) {
            if (res->getID() == id) {
                return res;
            }
        }
        return nullptr;
    }

    // Find member by ID
    LibraryMember* findMember(int id) {
        for (auto& mem : members) {
            if (mem.getID() == id) {
                return &mem;
            }
        }
        return nullptr;
    }

    // Borrow a resource
    bool borrowResource(int memberID, int resourceID) {
        LibraryMember* mem = findMember(memberID);
        LibraryResource* res = findResource(resourceID);

        if (!mem || !res) return false;
        if (!res->getAvailability()) return false;
        if (mem->getBorrowedCount() >= 5) return false; // Max 5 items

        if (mem->borrowResource(resourceID)) {
            res->setAvailability(false);
            saveToFile();
            return true;
        }
        return false;
    }

    // Return a resource
    bool returnResource(int memberID, int resourceID) {
        LibraryMember* mem = findMember(memberID);
        LibraryResource* res = findResource(resourceID);

        if (!mem || !res) return false;

        if (mem->returnResource(resourceID)) {
            res->setAvailability(true);
            saveToFile();
            return true;
        }
        return false;
    }

    // Get all resources as JSON array
    string getAllResourcesJSON() const {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < resources.size(); i++) {
            ss << resources[i]->toJSON();
            if (i < resources.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }

    // Get all members as JSON array
    string getAllMembersJSON() const {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < members.size(); i++) {
            ss << members[i].toJSON();
            if (i < members.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }

    // Get single resource JSON
    string getResourceJSON(int id) const {
        for (const auto& res : resources) {
            if (res->getID() == id) {
                return res->toJSON();
            }
        }
        return "{}";
    }

    // Get single member JSON
    string getMemberJSON(int id) const {
        for (const auto& mem : members) {
            if (mem.getID() == id) {
                return mem.toJSON();
            }
        }
        return "{}";
    }

    // Save data to file
    void saveToFile() {
        ofstream file("library_data.txt");
        if (!file.is_open()) return;

        // Save resources
        file << "RESOURCES:" << endl;
        for (const auto& res : resources) {
            file << res->getID() << "|";
            file << res->getType() << "|";
            file << res->getTitle() << "|";
            file << res->getAuthor() << "|";
            file << res->getExtraInfo() << "|";
            file << (res->getAvailability() ? "1" : "0") << endl;
        }

        // Save members
        file << "MEMBERS:" << endl;
        for (const auto& mem : members) {
            file << mem.getID() << "|" << mem.getName() << endl;
        }

        // Save borrowed items
        file << "BORROWED:" << endl;
        for (const auto& mem : members) {
            const auto& ids = mem.getBorrowedIDs();
            for (int id : ids) {
                file << mem.getID() << "|" << id << "|" << mem.getBorrowDate(id) << endl;
            }
        }

        file.close();
    }

    // Load data from file
    void loadFromFile() {
        ifstream file("library_data.txt");
        if (!file.is_open()) return;

        string line;
        string section = "";
        
        while (getline(file, line)) {
            if (line == "RESOURCES:") {
                section = "resources";
                continue;
            } else if (line == "MEMBERS:") {
                section = "members";
                continue;
            } else if (line == "BORROWED:") {
                section = "borrowed";
                continue;
            }

            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            vector<string> parts;

            while (getline(ss, token, '|')) {
                parts.push_back(token);
            }

            if (section == "resources" && parts.size() >= 6) {
                int id = stoi(parts[0]);
                string type = parts[1];
                string title = parts[2];
                string author = parts[3];
                string extra = parts[4];
                bool avail = (parts[5] == "1");

                LibraryResource* res = nullptr;
                if (type == "Book") {
                    // Parse ISBN and pages from extra
                    size_t pos = extra.find("|");
                    string isbn = (pos != string::npos) ? extra.substr(5, pos - 5) : "";
                    string pages = (pos != string::npos) ? extra.substr(pos + 6) : "0";
                    res = new Book(id, title, author, isbn, stoi(pages));
                } else if (type == "Magazine") {
                    int issue = 0;
                    if (extra.length() > 6) {
                        try { issue = stoi(extra.substr(6)); } catch (...) {}
                    }
                    res = new Magazine(id, title, author, issue);
                } else if (type == "DVD") {
                    int dur = 0;
                    if (extra.length() > 8) {
                        try { dur = stoi(extra.substr(8, extra.size() - 11)); } catch (...) {}
                    }
                    res = new DVD(id, title, author, dur);
                }

                if (res) {
                    res->setAvailability(avail);
                    resources.push_back(res);
                    if (id >= nextResourceID) nextResourceID = id + 1;
                }
            } else if (section == "members" && parts.size() >= 2) {
                int id = stoi(parts[0]);
                string name = parts[1];
                members.push_back(LibraryMember(id, name));
                if (id >= nextMemberID) nextMemberID = id + 1;
            } else if (section == "borrowed" && parts.size() >= 3) {
                int memID = stoi(parts[0]);
                int resID = stoi(parts[1]);
                LibraryMember* mem = findMember(memID);
                if (mem) {
                    mem->borrowResource(resID);
                }
            }
        }

        file.close();
    }

    // Get resource count
    int getResourceCount() const { return resources.size(); }

    // Get member count
    int getMemberCount() const { return members.size(); }
};

#endif // LIBRARY_H