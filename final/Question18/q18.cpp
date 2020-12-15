#include <iostream>
#include <vector>

typedef int ElemType;
class Double_stacks
{
private:
    int top[2], bot[2];      //stack top and bottom pointers for s1 and s2
    std::vector<ElemType> v; //array
    int max_num;             //array size: max num of elems in total
public:
    Double_stacks(int n) : max_num(n)
    {
        top[0] = -1;
        top[1] = max_num;
        v.assign(max_num, 0);
    }
    void push(int s, int x)
    {
        if (isFull(s))
        {
            double_capacity();
        }
        if (s == 1)
        {
            top[1]--;
            v[top[1]] = x;
        }
        else
        {
            top[0]++;
            v[top[0]] = x;
        }
    }
    ElemType pop(int s)
    {
        if (isEmpty(s))
        {
            return;
        }
        return v[top[s]];
    }

    bool isEmpty(int s)
    {
        if (s == 0 && top[0] == -1) // s1 is empty
        {
            return true;
        }
        if (s == 1 && top[1] == max_num) // s2 is empty
        {
            return true;
        }
        return false;
    }

    bool isFull(int s)
    {
        if (top[1] - top[0] == 1)
        {
            return true;
        }
        return false;
    }

    // Answer to (b): worst case O(n)
    // this is the double() implementation
    // use double_capacity() instead because C++ has built-in double()
    void double_capacity()
    {
        int doubled_max = max_num * 2;
        std::vector<ElemType> tmp;
        tmp.assign(doubled_max, 0);
        for (int i = 0; i < top[0]; ++i)
        {
            tmp[i] = v[i];
        }
        int j = doubled_max - 1;
        for (int i = max_num - 1; i >= top[1]; --i)
        {
            tmp[j] = v[i];
            --j;
        }
        v = tmp;
        max_num *= 2;
        return;
    }
};