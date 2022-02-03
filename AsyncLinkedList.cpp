
#include "AsyncLinkedList.h"

//#define USE_COMPARE_AND_SWAP
//#define USE_SPIN_LOCK

AsyncLinkedList::AsyncLinkedList()
{
}

AsyncLinkedList::~AsyncLinkedList()
{
    Clear();
}

void AsyncLinkedList::Insert(const int value)
{
    Node* newNode = new Node();
    newNode->m_value = value;
#ifdef USE_COMPARE_AND_SWAP

    while (true)
    {
        auto oldHead = m_head.load();
        newNode->m_next = oldHead;
        bool exchangedOk = m_head.compare_exchange_strong(oldHead, newNode);
        if (exchangedOk)
        {
            break;
        }
    }

#else
#ifndef USE_SPIN_LOCK
    std::lock_guard<std::mutex> lck(m_mutex);
#else    
    m_spinlock.lock();
#endif
    newNode->m_next.store(m_head);
    m_head = newNode;

#ifdef USE_SPIN_LOCK
    m_spinlock.unlock();
#endif    
#endif // CAS
}

void AsyncLinkedList::PrintLinkedList()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    const Node* n = m_head;
    while (n)
    {
        std::cout << n->m_value << " ";
        n = n->m_next;
    }
    std::cout << std::endl;
}

std::vector<int> AsyncLinkedList::GetLinkedListContents() const
{
    std::vector<int> res;
    const Node* n = m_head;
    while (n)
    {
        res.push_back(n->m_value);
        n = n->m_next;
    }
    return res;
}

void AsyncLinkedList::Clear()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    std::vector<const Node*> nodes;
    const Node* n = m_head;
    while (n)
    {
        nodes.push_back(n);
        n = n->m_next;
    }
    for (const auto& elem : nodes)
    {
        delete elem;
    }
    m_head = nullptr;
}
