
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#define LEN         10000000
#define JSTR        ("{ \"Name\" : \"Boo\", \"Age\" : 28}")

uint64_t get_ns_time()
{
    struct timespec tns;
    clock_gettime(CLOCK_REALTIME, &tns);
    
    return ((uint64_t) tns.tv_sec * 1000000000L) + tns.tv_nsec;
}

using namespace rapidjson;

void test_1()
{
    uint64_t btime = get_ns_time();
    
    Document d;
    for (size_t i = 0; i < LEN; i++)
    {
        d.Parse(JSTR);
    }
    
    uint64_t etime = get_ns_time();
    
    printf("total spend %lu nanos\n", etime - btime);
}

void test_2()
{
    uint64_t btime = get_ns_time();
    
    for (size_t i = 0; i < LEN; i++)
    {
        Document d;
        d.Parse(JSTR);
    }
    
    uint64_t etime = get_ns_time();
    
    printf("total spend %lu nanos\n", etime - btime);
}

void test_3()
{
    uint64_t btime = get_ns_time();
    
    typedef GenericDocument<UTF8<>, MemoryPoolAllocator<>, MemoryPoolAllocator<> > DocumentType;
    char valueBuffer[4096];
    char parseBuffer[1024];
    for (size_t i = 0; i < LEN; i++)
    {
        MemoryPoolAllocator<> valueAllocator(valueBuffer, sizeof(valueBuffer));
        MemoryPoolAllocator<> parseAllocator(parseBuffer, sizeof(parseBuffer));
        DocumentType d(&valueAllocator, sizeof(parseBuffer), &parseAllocator);
        d.Parse(JSTR);
    }
    
    uint64_t etime = get_ns_time();
    
    printf("total spend %lu nanos\n", etime - btime);
}

/**
 *  测试结果:

    ===== test one =====
    total spend 17447060362 nanos
    ===== test two =====
    total spend 20784218501 nanos
    ===== test thr =====
    total spend 17885584921 nanos
 */

int main()
{
    printf("===== test one =====\n");
    test_1();
    
    printf("===== test two =====\n");
    test_2();
    
    printf("===== test thr =====\n");
    test_3();
    
    return 0;
}