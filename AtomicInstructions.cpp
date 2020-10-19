#include "AtomicInstructions.h"
#include <iostream>
#include <string>




AtomicInstructions::AtomicInstructions()
{

}

AtomicInstructions::~AtomicInstructions()
{
    for (auto& elem : m_readerThreads)
    {
        elem.join();
    }
}

void AtomicInstructions::Run()
{
    m_listHead = new Node();
    CreateReaderThreads();
    CreateWriteThread();
    while (true)
    {
        
    }
}

void AtomicInstructions::CreateReaderThreads()
{
    int numThreadsLeft = m_numReaderThreads;
    while (numThreadsLeft--)
    {
        std::string name = "Reader " + std::to_string(numThreadsLeft);
        auto t = std::thread(ReaderFunctionObject(name, m_listHead));
        m_readerThreads.push_back(std::move(t));
    }
}

void AtomicInstructions::CreateWriteThread()
{
    std::string name = "Writer";
    auto t = std::thread(WriterFunctionObject(name, m_listHead));
    m_writerThread = std::move(t);
}

AtomicInstructions::WriterFunctionObject::WriterFunctionObject(const std::string& name, AtomicInstructions::Node* node) : m_name(name), m_listHead(node)
{

}

void AtomicInstructions::WriterFunctionObject::operator()()
{
    using namespace std;
    std::cout << "Writing..." << m_name << std::endl;
    while (true)
    {
        cout << "Enter number to insert into linked list" << endl;
        int value;
        cin >> value;
        InsertIntoLinkedList(value);
        PrintLinkedList();
    }
}

void AtomicInstructions::WriterFunctionObject::PrintLinkedList()
{
    auto iter = m_listHead->m_next;
    while (iter)
    {
        std::cout << iter->m_value << " ";
        iter = iter->m_next;
    }
    std::cout << std::endl;
}

AtomicInstructions::ReaderFunctionObject::ReaderFunctionObject(const std::string& name, AtomicInstructions::Node* node) : m_name(name), m_listHead(node)
{

}

void AtomicInstructions::ReaderFunctionObject::operator()() const
{
    std::cout << "Reading..." << m_name << std::endl;
    while (true)
    {
        const auto* nodeIter = m_listHead;
        while (nodeIter)
        {
            nodeIter = nodeIter->m_next;
        }
    }
}
