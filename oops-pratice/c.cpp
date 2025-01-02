#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;    // normal member
    int rollNo;     // normal member
    int* marks;     // dynamic array
    int size;       // size of array
    
public:
    // Constructor
    Student(string n, int roll, int s) {
        name = n;
        rollNo = roll;
        size = s;
        marks = new int[size];
        for(int i = 0; i < size; i++) {
            marks[i] = 0;
        }
    }
    
    // Copy Constructor
    Student(const Student &obj) {
        cout << "Copy Constructor called" << endl;
        // Copy normal members
        name = obj.name;
        rollNo = obj.rollNo;
        size = obj.size;
        
        // Deep copy of dynamic array
        marks = new int[size];
        for(int i = 0; i < size; i++) {
            marks[i] = obj.marks[i];
        }
    }
    
    // Assignment Operator
    Student& operator=(const Student &obj) {
        cout << "Assignment Operator called" << endl;
        if(this != &obj) {  // Check for self-assignment
            // Free existing memory
            delete[] marks;
            
            // Copy normal members
            name = obj.name;
            rollNo = obj.rollNo;
            size = obj.size;
            
            // Deep copy of dynamic array
            marks = new int[size];
            for(int i = 0; i < size; i++) {
                marks[i] = obj.marks[i];
            }
        }
        return *this;
    }
    
    // Set marks
    void setMarks(int index, int mark) {
        if(index < size) {
            marks[index] = mark;
        }
    }
    
    // Display
    void display() {
        cout << "Name: " << name << endl;
        cout << "Roll No: " << rollNo << endl;
        cout << "Marks: ";
        for(int i = 0; i < size; i++) {
            cout << marks[i] << " ";
        }
        cout << endl;
    }
    
    // Destructor
    ~Student() {
        delete[] marks;
    }
};

int main() {
    // Testing Copy Constructor
    Student s1("Pragun", 101, 3);
    s1.setMarks(0, 85);
    s1.setMarks(1, 90);
    s1.setMarks(2, 95);
    
    Student s2 = s1;    // Copy Constructor called
    
    cout << "\nAfter Copy Construction:" << endl;
    cout << "s1's data:" << endl;
    s1.display();
    cout << "s2's data:" << endl;
    s2.display();
    
    // Testing Assignment Operator
    Student s3("Bhutani", 102, 3);
    s3 = s1;            // Assignment Operator called
    
    cout << "\nAfter Assignment:" << endl;
    cout << "s1's data:" << endl;
    s1.display();
    cout << "s3's data:" << endl;
    s3.display();
    
    // Modifying s2 and s3 to show deep copy worked
    s2.setMarks(0, 70);
    s3.setMarks(1, 75);
    
    cout << "\nAfter Modifications:" << endl;
    cout << "s1's data:" << endl;
    s1.display();
    cout << "s2's data:" << endl;
    s2.display();
    cout << "s3's data:" << endl;
    s3.display();
    
    return 0;
}