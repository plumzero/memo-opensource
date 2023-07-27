
#include <readerwriterqueue.h>
#include <cstdio>

#define mcl   moodycamel

int main()
{
    {
        size_t capacity = 5;
        int * front{};
        size_t i;
        int num;
        
        mcl::ReaderWriterQueue<int> Q(capacity);
        
        printf("===== the first round enqueue/dequeue test =====\n");
        for (i = 0; i < Q.max_capacity(); i++) {
            Q.enqueue(i);
        }
        
        front = Q.peek();
        printf("front=%p\n", (void*)front);
        
        for (i = 0; i < Q.max_capacity(); i++) {
            front = Q.peek();
            printf("dequeue: max_capacity=%lu,size=%lu,front=%p\n", Q.max_capacity(), Q.size_approx(), (void*)front);
            Q.try_dequeue(num);
        }
        
        printf("===== the second round enqueue/dequeue test(the same ReaderWriterQueue with the first round) =====\n");
        for (i = 0; i < Q.max_capacity(); i++) {
            Q.enqueue(i);
        }
        
        front = Q.peek();
        printf("front=%p\n", (void*)front);
        
        for (i = 0; i < Q.max_capacity(); i++) {
            front = Q.peek();
            printf("dequeue: max_capacity=%lu,size=%lu,front=%p\n", Q.max_capacity(), Q.size_approx(), (void*)front);
            Q.try_dequeue(num);
        }
        
        printf("\n\n");
    }
    
    {
        size_t capacity = 5000;
        int * front{};
        size_t i;
        int num;
        long addr[2]{};
        bool crash{false};
        
        mcl::ReaderWriterQueue<int> Q(capacity);
        
        printf("===== the third round enqueue/dequeue(another ReaderWriterQueue) test ====\n");
        printf("max_capacity=%lu,size=%lu\n", Q.max_capacity(), Q.size_approx());
        Q.enqueue(10);
        front = Q.peek();
        addr[0] = reinterpret_cast<long>(front);

        printf("front=%p, addr[0]=0x%lx\n", (void*)front, addr[0]);
        
        for (i = 0; i < 1000000000; i++) {
            Q.try_dequeue(num);
            Q.enqueue(i);
            front = Q.peek();
            
            if (crash) {
                addr[1] = reinterpret_cast<long>(front);
                printf("break! i=%lu,max_capacity=%lu,size=%lu,addr[0]=0x%lx,addr[1]=0x%lx\n",
                        i, Q.max_capacity(), Q.size_approx(), addr[0], addr[1]);
                break;
            }
            
            if (addr[0] == reinterpret_cast<long>(front)) {
                crash = true;
                printf("find! i=%lu\n", i);
            }
        }
        
        printf("i=%lu, addr[0]=0x%lx, addr[1]=0x%lx\n", i, addr[0], addr[1]);
    }

    printf("\n\n===== test over =====\n");
    
    return 0;
}
