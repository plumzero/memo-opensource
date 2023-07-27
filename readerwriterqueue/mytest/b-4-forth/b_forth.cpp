
#include <readerwriterqueue.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

#define mcl   moodycamel

#define TOTAL 6000

int main()
{
    {
        printf("===== first round: one reader thread/one writer thread operate concurrently  =====\n");
        mcl::BlockingReaderWriterQueue<int> Q(TOTAL);

        std::thread writer([&Q](){
            for (int i = 0; i != TOTAL; i++) {
                Q.enqueue(i);
                // std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        std::thread reader([&Q](){
            int num;
            for (int i = 0; i != TOTAL;) {
                if (Q.wait_dequeue_timed(num, std::chrono::milliseconds(5))) {
                    ++i;
                }
            }
        });

        writer.join();
        reader.join();

        assert(Q.size_approx() == 0);
    }
    
    {
        printf("===== second round: write first and then three reader thread operate concurrently  =====\n");
        mcl::BlockingReaderWriterQueue<int> Q(TOTAL);
        
        std::atomic<int> counter{0};
        
        for (int i = 0; i != TOTAL; i++) {
            Q.enqueue(i);
        }
        
        int counter1{0};
        int counter2{0};
        int counter3{0};
        
        std::thread reader1([&Q, &counter](int & ctr) {
            int num;
            while (true) {
                if (Q.wait_dequeue_timed(num, std::chrono::milliseconds(5))) {
                    ++counter;
                    ++ctr;
                }
                if (counter == TOTAL) {
                    break;
                }
            }
        }, std::ref(counter1));
        
        std::thread reader2([&Q, &counter](int & ctr) {
            int num;
            while (true) {
                if (Q.wait_dequeue_timed(num, std::chrono::milliseconds(5))) {
                    ++counter;
                    ++ctr;
                }
                if (counter == TOTAL) {
                    break;
                }
            }
        }, std::ref(counter2));
        
        
        std::thread reader3([&Q, &counter](int & ctr) {
            int num;
            while (true) {
                if (Q.wait_dequeue_timed(num, std::chrono::milliseconds(5))) {
                    ++counter;
                    ++ctr;
                }
                if (counter == TOTAL) {
                    break;
                }
            }
        }, std::ref(counter3));
        
        reader1.join();
        reader2.join();
        reader3.join();

        assert(Q.size_approx() == 0);
        
        printf("counter1=%d, counter2=%d, counter3=%d\n", counter1, counter2, counter3);
        printf("counter1+counter2+counter3=%d\n", counter1 + counter2 + counter3);
        printf("atomic<int> counter=%d\n", counter.load(std::memory_order_relaxed));
    }

    printf("===== test over =====\n");
    
    return 0;
}
