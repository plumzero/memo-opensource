
## 使用说明

头文件引用即可。

## 测试目录说明

1-first
    ReaderWriterQueue 的简单使用。
    ReaderWriterQueue 会对传入的容量进行一点小操作，实际容量比传入值要大一些。

2-second
    ReaderWriterQueue 超出设置容量(capacity)时情况。 
    超出容量时，会自动扩容。

3-third
    ReaderWriterQueue 生产/消费时的内存地址变化。
    
    可以看出 ReaderWriterQueue 是一个环形队列。而且，如果你传入的 capacity 过大(即实际上用不了这么多)，在实际运行时并不会真的分配 capacity 那么大的内存。    
    **为了防止在使用过程中因不断分配内存而产生的资源消耗，可以在创建队列时设置队列的容量。又因为在实际运行时并不一定真的会分配 capacity 那么大的内存，所以即使设置的容量很大也没有关系。**


关于对 ReaderWriterQueue 线程安全的测试:
    因为 ReaderWriterQueue 并没有提供类似于 wait(等待队列中有元素)的接口，所以 ReaderWriterQueue 并不适用于并发程序(如典型的读写线程)，也因此对 ReaderWriterQueue 队列的线程安全测试是没有意义的。


----
1-b-first
2-b-second
3-b-third
    BlockingReaderWriterQueue 对应于 ReaderWriterQueue 中的测试。
    测试结果一致。
    
4-b-forth
    BlockingReaderWriterQueue 的线程安全测试。
    
    BlockingReaderWriterQueue 是线程安全的。
    




