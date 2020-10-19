#pragma once

#include <thread>
#include <vector>
#include <string>
#include <iostream>

class AtomicInstructions
{
public:

    AtomicInstructions();
    ~AtomicInstructions();
    void Run();

    struct Node
    {
        int m_value = INT_MAX;
        Node* m_next = nullptr;
    };

private:

    class ReaderFunctionObject
    {
    public:
        ReaderFunctionObject(const std::string& name, AtomicInstructions::Node* node);

        void operator()() const;

    private:

        AtomicInstructions::Node* m_listHead = nullptr;
        std::string m_name;
    };

    class WriterFunctionObject
    {
    public:
        WriterFunctionObject(const std::string& name, AtomicInstructions::Node* node);

        void operator()();

    private:

        AtomicInstructions::Node* m_listHead = nullptr;
        std::string m_name;

        void InsertIntoLinkedList(int value)
        {
            auto iter = m_listHead;
            while (iter)
            {
                if (iter->m_next == nullptr)
                {
                    InsertNodeAfter(iter, value);
                    return; 
                }
                else if (iter->m_next->m_value >= value)
                {
                    auto nextNext = iter->m_next;
                    InsertNodeAfter(iter, value);
                    iter->m_next->m_next = nextNext;
                    return;
                }
                else
                {
                    iter = iter->m_next;
                }
            }
        }



        void InsertNodeAfter(AtomicInstructions::Node* iter, int value)
        {
            AtomicInstructions::Node* newNode = new AtomicInstructions::Node();
            iter->m_next = newNode;
            newNode->m_value = value;
        }

        void PrintLinkedList();
    };

    std::vector<std::thread> m_readerThreads;
    std::thread m_writerThread;

    Node* m_listHead = nullptr;

    static const int m_numReaderThreads = 5;
    void CreateReaderThreads();
    void CreateWriteThread();
};