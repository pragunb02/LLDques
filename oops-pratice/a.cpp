#include <bits/stdc++.h>
using namespace std;

class Student
{
    // private:
public:
    int val;
    string name;
    Student()
    {
        cout << "Hello" << endl;
    }
    Student(string name, int val)
    {
        this->name = name;
        this->val = val;
        cout << "Hi" << endl;
    }
};

class Firstyr : public Student
{
    // private:
public:
    string lastname;
    int num;
    Firstyr(string name, int val, string lastname, int num) : Student(name, val)
    {
        this->lastname = lastname;
        this->num = num;
    }
};

int main()
{
    Student s("Pragun", 12);
    cout << (s.name) << endl;
    Firstyr f("NONO",14,"Bhutani", 13);
    cout << (f.name) << endl;
    cout << (f.lastname) << endl;
}