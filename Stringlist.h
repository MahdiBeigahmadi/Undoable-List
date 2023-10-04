// Stringlist.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Mahdi Beigahmadi>
// St.# : <301570853>
// Email: <mba188@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class Stringlist
{
    int cap;
    string *arr;
    int sz;

    bool removeAllFlag = false;
    constexpr static unsigned int INITIAL_CAPACITY = 300;
    bool assignmentOperator = false;
    string removeAllArray[INITIAL_CAPACITY] = {""};

    unsigned int removeSizeTracker = 0;

    class Node
    {
    public:
        string element;
        string valueHolder;
        unsigned int indexTracker;
        unsigned int backUpSize;
        string *backUpArray;
        Node *next = nullptr;

        Node(string value1, string value2)
            : element(value1), valueHolder(value2), indexTracker(0), backUpSize(0), backUpArray(nullptr), next(nullptr) {}

        Node(string *arrBackup, unsigned int szBackup)
            : element(), valueHolder(), indexTracker(0), backUpSize(szBackup), backUpArray(arrBackup), next(nullptr) {}

        ~Node()
        {
            if (backUpArray != nullptr)
                delete[] backUpArray;
        }
    };
    Node *headLinkedList = nullptr;

    void bounds_error(const string &s) const
    {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }

    void check_bounds(const string &s, int i) const
    {
        if (i < 0 || i >= sz)
        {
            bounds_error(s);
        }
    }

    void copy(const string *other)
    {
        for (int i = 0; i < sz; i++)
        {
            arr[i] = other[i];
        }
    }

    void check_capacity()
    {
        if (sz == cap)
        {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
    }

public:
    Stringlist()
        : cap(10), arr(new string[cap]), sz(0)

    {
    }

    Stringlist(const Stringlist &other)
        : cap(other.cap), arr(new string[cap]), sz(other.sz)
    {

        copy(other.arr);
    }

    ~Stringlist()
    {
        Node *current = headLinkedList;
        while (current)
        {
            Node *nextNode = current->next;
            delete current;
            current = nextNode;
        }
        headLinkedList = nullptr;
        delete[] arr;
    }

    Stringlist &operator=(const Stringlist &other)
    {

        if (this != &other)
        {

            string *backup = new string[sz];
            for (unsigned int index = 0; index < sz; index++)
            {
                backup[index] = arr[index];
            }

            Node *newNode = new Node(backup, sz);
            if (!headLinkedList)
            {
                headLinkedList = newNode;
            }
            else
            {
                Node *temp = headLinkedList;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }

            cap = other.cap;
            sz = other.sz;
            delete[] arr;
            arr = new string[cap];
            for (unsigned int index = 0; index < sz; index++)
            {
                arr[index] = other.arr[index];
            }
            assignmentOperator = true;
        }
        return *this;
    }

    int size() const { return sz; }

    bool empty() const { return size() == 0; }

    int capacity() const { return cap; }

    string get(int index) const
    {
        check_bounds("get", index);
        return arr[index];
    }

    int index_of(const string &s) const
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                return i;
            }
        }
        return -1;
    }

    bool contains(const string &s) const
    {
        return index_of(s) != -1;
    }

    string to_string() const
    {
        string result = "{";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    void set(int index, string value)
    {
        check_bounds("set", index);
        arr[index] = value;
        Node *newNode = new Node("REMOVE", value);
        if (!headLinkedList)
        {
            headLinkedList = newNode;
            newNode->indexTracker = index;
        }
        else
        {
            Node *temp = headLinkedList;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->indexTracker = index;
        }
    }

    void insert_before(int index, const string &s)
    {

        if (removeAllFlag == false)
        {
            Node *newNode = new Node("REMOVE", s);
            newNode->indexTracker = index;

            if (!headLinkedList)
            {
                headLinkedList = newNode;
            }
            else
            {

                Node *temp = headLinkedList;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }

        if (index < 0 || index > sz)
        {
            bounds_error("insert_before");
        }

        check_capacity();

        for (int i = sz; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;

        sz++;

        removeAllArray[index] = s;
        removeSizeTracker++;
    }

    void insert_back(const string &s)
    {
        insert_before(size(), s);
    }

    void insert_front(const string &s)
    {
        insert_before(0, s);
    }

    void remove_at(int index)
    {

        Node *newNode = new Node("INSERT", arr[index]);
        if (!headLinkedList)
        {
            headLinkedList = newNode;
            newNode->indexTracker = index;
        }
        else
        {
            Node *temp = headLinkedList;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->indexTracker = index;
        }

        check_bounds("remove_at", index);
        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;
    }

    void remove_all()
    {
        removeAllFlag = true;
        while (sz > 0)
        {

            remove_at(sz - 1);
        }
    }

    bool remove_first(const string &s)
    {
        int index = index_of(s);
        if (index == -1)
        {
            return false;
        }
        remove_at(index);
        return true;
    }

    bool undo();
};

ostream &operator<<(ostream &os, const Stringlist &lst)
{
    return os << lst.to_string();
}

bool operator==(const Stringlist &a, const Stringlist &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a.get(i) != b.get(i))
        {
            return false;
        }
    }
    return true;
}

bool operator!=(const Stringlist &a, const Stringlist &b)
{
    return !(a == b);
}

bool Stringlist::undo()
{
    bool success = false;

    if (removeAllFlag == true)
    {
        Node *current = headLinkedList;
        while (current)
        {
            Node *nextNode = current->next;
            delete current;
            current = nextNode;
        }
        headLinkedList = nullptr;
    }

    else
    {
        if (!headLinkedList)
        {
            cerr << "The list is empty!" << endl;
            return success;
        }
        Node *temp = headLinkedList;
        Node *prev = nullptr;
        while (temp->next)
        {
            prev = temp;
            temp = temp->next;
        }

        if (temp->element == "INSERT")
        {
            // remove operation at array ---> inert operation to undo
            insert_before(temp->indexTracker, temp->valueHolder);
            success = true;
        }
        else if (temp->element == "REMOVE")
        {
            // insert operation at array ---> remove operation to undo
            remove_at(temp->indexTracker);
            success = true;
        }
        else if (assignmentOperator == true)
        { // I helped chatGPT to write the = operator in this function
            if (arr != temp->backUpArray)
            {
                delete[] arr;
            }
            arr = temp->backUpArray;
            sz = temp->backUpSize;
            cap = sz;

            if (headLinkedList == temp)
                headLinkedList = nullptr;
            else
            {
                Node *pre = headLinkedList;
                while (pre->next != temp)
                    pre = pre->next;
                pre->next = nullptr;
            }
            success = true;
            assignmentOperator = false;
        }

        // we need to adjust pointers and delete the last action node.
        if (prev)
        {
            prev->next = nullptr;
        }
        else
        {
            headLinkedList = nullptr;
        }
    }

    if (removeAllFlag == true)
    {
        for (unsigned int index = 0; index < INITIAL_CAPACITY; index++)
        {

            if (removeAllArray[index] == "")
            {
                break;
            }
            else
            {
                insert_before(index, removeAllArray[index]);
            }
        }
    }

    removeAllFlag = false; // reset removeAllFlag to its first state for other remove alls
    return success;
}
