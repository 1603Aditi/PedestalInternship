#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

struct Student {
    string rollNo;
    string name;
    float marks;
    string grade;
};

string filename = "students.txt";
string calculateGrade(float marks,vector<int>&inpmarks, vector<string>&inpgrades) {
    int n=inpmarks.size();
    string grades="";
    for(int i=0;i<n;i++){
        if(marks>=inpmarks[i]){
            grades=inpgrades[i];
            break;
        }
        else{
            continue;
        }
    }
    return grades;
}

bool isRollNoUnique(string rollNo) {
    ifstream inFile(filename);
    string line, tempRoll;
    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, tempRoll, ','); 
        if (tempRoll == rollNo) return false;
    }
    return true;
}

void addStudent(vector<int>&inpmarks, vector<string>&inpgrades) {
    Student s;
    cout << "Enter Roll No: ";
    cin >> s.rollNo;

    if (!isRollNoUnique(s.rollNo)) {
        cout << "Error: Roll No already exists.\n";
        return;
    }
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, s.name);
    cout << "Enter Marks: ";
    cin >> s.marks;
    if (s.marks < 0 || s.marks > 100) {
        cout << "Invalid marks! Must be between 0 and 100.\n";
        return;
    }
    s.grade = calculateGrade(s.marks,inpmarks,inpgrades);
    ofstream outFile(filename, ios::app);
    outFile << s.rollNo << "," << s.name << "," << s.marks << "," << s.grade << "\n";
    outFile.close();
    cout << "Student added successfully!\n";
}

void viewStudents() {
    ifstream inFile(filename);
    Student s;
    string line;
    cout << "Roll No\tName\tMarks\tGrade\n";
    while (getline(inFile, line)) {
        stringstream ss(line);
        string marksStr, gradeStr;
        getline(ss, s.rollNo, ',');
        getline(ss, s.name, ',');
        getline(ss, marksStr, ',');
        getline(ss, gradeStr);
        s.marks = stof(marksStr);
        s.grade = gradeStr;
        cout << s.rollNo << "\t" << s.name << "\t" << s.marks << "\t" << s.grade << "\n";
    }
    inFile.close();
}

vector<Student> readAllStudents() {
    ifstream inFile(filename);
    vector<Student> students;
    string line;
    while (getline(inFile, line)) {
        Student s;
        string marksStr, gradeStr;
        stringstream ss(line);
        getline(ss, s.rollNo, ',');
        getline(ss, s.name, ',');
        getline(ss, marksStr, ',');
        getline(ss, gradeStr);
        s.marks = stof(marksStr);
        s.grade = gradeStr;
        students.push_back(s);
    }
    inFile.close();
    return students;
}

void saveAllStudents(const vector<Student>& students) {
    ofstream outFile(filename);
    for (const auto& s : students) {
        outFile << s.rollNo << "," << s.name << "," << s.marks << "," << s.grade << "\n";
    }
    outFile.close();
}

void searchStudent() {
    string roll;
    cout << "Enter Roll No to search: ";
    cin >> roll;
    bool found = false;
    vector<Student> students = readAllStudents();
    for (const auto& s : students) {
        if (s.rollNo == roll) {
            cout << "Found: " << s.name << " | Marks: " << s.marks << " | Grade: " << s.grade << "\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found.\n";
}

void updateStudent(vector<int>&inpmarks, vector<string>&inpgrades) {
    string roll;
    cout << "Enter Roll No to update: ";
    cin >> roll;
    bool found = false;
    vector<Student> students = readAllStudents();
    for (auto& s : students) {
        if (s.rollNo == roll) {
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, s.name);
            cout << "Enter new marks: ";
            cin >> s.marks;
            if (s.marks < 0 || s.marks > 100) {
                cout << "Invalid marks! Must be between 0 and 100.\n";
                return;
            }
            s.grade = calculateGrade(s.marks,inpmarks,inpgrades);
            found = true;
            break;
        }
    }
    if (found) {
        saveAllStudents(students);
        cout << "Record updated.\n";
    } else {
        cout << "Student not found.\n";
    }
}

void deleteStudent() {
    string roll;
    cout << "Enter Roll No to delete: ";
    cin >> roll;
    vector<Student> students = readAllStudents();
    auto it = remove_if(students.begin(), students.end(), [roll](Student s) {
        return s.rollNo == roll;
    });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveAllStudents(students);
        cout << "Student deleted.\n";
    } else {
        cout << "Student not found.\n";
    }
}

void sortStudents(){
    int choice;
    cout << "Sort by: 1. Marks  2. Name\nEnter choice: ";
    cin >> choice;
    vector<Student> students = readAllStudents();
    if (choice == 1) {
        sort(students.begin(), students.end(), [](Student a, Student b) {
            return a.marks > b.marks;
        });
    } else {
        sort(students.begin(), students.end(), [](Student a, Student b) {
            return a.name < b.name;
        });
    }

    cout << "Sorted Records:\n";
    for (auto& s : students) {
        cout << s.rollNo << "\t" << s.name << "\t" << s.marks << "\t" << s.grade << "\n";
    }
}

int main() {

    cout<<"<-----------------------------Welcome to Student Record Management System----------------------------->"<<endl;
    cout<<"Please tell the grading System to us"<<endl;
    cout<<"Firstly input the minimum marks for that grades then input the grade"<<endl;
    cout<<"Start from the highest grade to lowest"<<endl;
    cout<<"Enter the last grade as [FF]"<<endl;
    vector<int>inpmarks;
    vector<string>inpgrades;
    int inpmark;
    string inpgrade;
    while(inpgrade!="FF"){
    cin>>inpmark;
    inpmarks.push_back(inpmark);
    cin>>inpgrade;
    inpgrades.push_back(inpgrade);
    }

    int choice;
    do {
        cout << "\n---Student Record Management System---\n";
        cout << "1. Add Student\n2. View All\n3. Search\n4. Update\n5. Delete\n6. Sort\n0. Exit\nEnter choice: ";

        cin >> choice;
        switch (choice) {
            case 1: addStudent(inpmarks,inpgrades); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(inpmarks,inpgrades); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again!\n"; break;

        }
    } 
    while (choice != 0);

    return 0;
}
