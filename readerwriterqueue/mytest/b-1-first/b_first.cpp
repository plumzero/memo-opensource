
#include <readerwriterqueue.h>
#include <cassert>
#include <iostream>

#define mcl   moodycamel

int main()
{
    size_t capacity = 10;
    
    mcl::BlockingReaderWriterQueue<int> Q(capacity);
    
    std::cout << "capacity=" << Q.max_capacity() << ", size=" << Q.size_approx() << std::endl;
    
    Q.enqueue(17);
    bool bl = Q.try_enqueue(18);
    assert(bl);
    std::cout << "capacity=" << Q.max_capacity() << ", size=" << Q.size_approx() << std::endl;
    
    int num;
    bl = Q.try_dequeue(num);
    assert(bl && num == 17);
    std::cout << "capacity=" << Q.max_capacity() << ", size=" << Q.size_approx() << std::endl;
    
    bl = Q.try_dequeue(num);
    assert(bl && num == 18);
    std::cout << "capacity=" << Q.max_capacity() << ", size=" << Q.size_approx() << std::endl;

    std::cout << "===== test over =====" << std::endl;
    
    return 0;
}
