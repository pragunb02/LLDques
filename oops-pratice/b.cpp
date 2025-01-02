#include <bits/stdc++.h>
using namespace std;

class A
{
public:
    int a;
    A(int a)
    {
        this->a = a;
    }
    void show()
    {
        cout << this->a << endl;
    }
};

class B : virtual public A
{
protected:
    B(int val) : A(val)
    {
    }
};

class C : virtual public A
{
protected:
    C(int val) : A(val)
    {
    }
};

class D : public B, public C
{
public:
    D(int val) : B(val), C(val), A(val)
    {
    }
};

int main()
{
    D obj(5);
    obj.show();
}
