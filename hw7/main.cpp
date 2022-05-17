#include "ringbuffer.hpp"
#include <iostream>
#include <vector>
using namespace __detail;

int main()
{
    RingBuffer<int> buffer;
    std::vector<int> b = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    // buffer.print();
    // buffer.write(b[0]);
    // buffer.print();
    buffer.write_multi(9, b);
    /* buffer.print();
    buffer.write(b[0]);
    buffer.print();
    size_t cap = buffer.get_capacity();
    std::cout << cap << std::endl;

    std::vector<int> a(7);
    bool flag = buffer.read_multi(10, a);
    // buffer.print();
    std::cout << flag << std::endl;
    for (int i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << std::endl;
    }
    std::cout << a.size() << std::endl;

    bool empty = buffer.is_empty();
    std::cout << empty << std::endl;*/

    // int flag = 0;
    for (__iterator<int> i = buffer.begin(); i != buffer.end(); i ++)
    {
        std::cout << *i << "\n";
    }
}