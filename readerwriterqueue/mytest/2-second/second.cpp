
#include <readerwriterqueue.h>
#include <cstdio>
#include <cassert>

#define mcl   moodycamel

int main()
{
    size_t capacity = 10;
    int * front{};
    
    mcl::ReaderWriterQueue<int> Q(capacity);
    
    bool bl;
    
    size_t i;
    for (i = 0; i < capacity; i++) {
        bl = Q.enqueue(i);
        assert(bl == true);
        printf("max_capacity=%lu,size=%lu\n", Q.max_capacity(), Q.size_approx());
    }
    
    front = Q.peek();
    printf("front=%p\n", (void*)front);

    for (; i < Q.max_capacity(); i++) {
        bl = Q.enqueue(i);
        assert(bl == true);
        printf("...max_capacity=%lu,size=%lu\n", Q.max_capacity(), Q.size_approx());
    }
    
    front = Q.peek();
    printf("front=%p\n", (void*)front);
    
    /** 会自动扩大容量 */
    bl = Q.enqueue(i);
    assert(bl == true);
    
    front = Q.peek();
    printf("front=%p\n", (void*)front);
    
    printf("......max_capacity=%lu,size=%lu\n", Q.max_capacity(), Q.size_approx());

    printf("===== test over =====\n");
    
    return 0;
}
