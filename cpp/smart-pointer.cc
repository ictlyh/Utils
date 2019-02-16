#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class SmartPointer {
public:
    SmartPointer():ptr(nullptr),pcnt(nullptr){}
    SmartPointer(T* t)
    {
        ptr = t;
        if (nullptr == t)
        {
            pcnt = nullptr;
        }
        else
        {
            pcnt = new size_t(1);
        }
        cout << "construct " << this << " points to " << ptr << ", use_count " << *pcnt << endl;
    }
    SmartPointer(const SmartPointer& rhs)
    {
        ptr = rhs.ptr;
        pcnt = rhs.pcnt;
        ++(*pcnt);
        cout << "construct " << this << " points to " << ptr << ", use_count " << *pcnt << endl;
    }
    SmartPointer& operator=(const SmartPointer& rhs)
    {
        if (this != &rhs)
        {
            if (ptr && pcnt)
            {
                --(*pcnt);
                if (!*pcnt)
                {
                    delete ptr;
                    delete pcnt;
                }
            }
            ptr = rhs.ptr;
            pcnt = rhs.pcnt;
            ++(*pcnt);
        }
        cout << "set " << this << " points to " << ptr << ", use_count " << *pcnt << endl;
        return *this;
    }
    T* operator->()
    {
        return ptr;
    }
    T& operator*()
    {
        return *ptr;
    }
    ~SmartPointer()
    {
        --(*pcnt);
        cout << "destruct " << this << " points to " << ptr << ", use_count " << *pcnt << endl;
        if (!*pcnt)
        {
            delete ptr;
            delete pcnt;
        }
    }
    size_t use_count()
    {
        return *pcnt;
    }
private:
    T* ptr;
    size_t* pcnt;
};

class MyClass
{
public:
    MyClass(int i):val(i)
    {
        cout << "construct " << this << " to " << i << endl;
    }
    ~MyClass()
    {
        cout << "destruct " << this << endl;
    }
private:
    int val;
};

void func(vector<SmartPointer<MyClass>> sps)
{
    return;
}

int main()
{
    MyClass* p = new MyClass(10);
    SmartPointer<MyClass> sp(p);
    vector<SmartPointer<MyClass>> vec(5, sp);
    func(vec);
    return 0;
}