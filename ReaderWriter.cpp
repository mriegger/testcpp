import ReaderWriter;
#include <algorithm>
#include <iostream>
#include <condition_variable>
#include <cassert>

void ReaderWriter::ReaderThread(const int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(m_counterMutex);
        while (m_resourceCounter == -1)
        {
            m_readPhase.wait(lck);
        }
        m_resourceCounter++;
        lck.unlock();

        // Read Data...
        //        std::cout << "Reading data" << id << " "  <<  m_resourceCounter << std::endl;

        lck.lock();
        m_resourceCounter--;
        if (m_resourceCounter == 0)
        {
            m_writePhase.notify_one();
        }
        lck.unlock();
    }
}

void ReaderWriter::WriterThread(const int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(m_counterMutex);
        while (m_resourceCounter != 0)
        {
            m_writePhase.wait(lck);
        }
        m_resourceCounter = -1;
        lck.unlock();

        // write data ...


        lck.lock();
        m_resourceCounter = 0;
        m_writePhase.notify_one();
        m_readPhase.notify_all();

        lck.unlock();
    }
}


void ReaderWriter::Run()
{
    SpawnThreads();

    JoinThreads();
}

void ReaderWriter::SpawnThreads()
{
    for (int i = 0; i < NumReaderThreads; ++i)
    {
        m_readerThreads.push_back(std::thread(&ReaderWriter::ReaderThread, this, i));
    }

    for (int i = 0; i < NumWriterThreads; ++i)
    {
        m_writerThreads.push_back(std::thread(&ReaderWriter::WriterThread, this, i));
    }
}

void ReaderWriter::JoinThreads()
{
    for_each(m_readerThreads.begin(), m_readerThreads.end(), [](auto& t) {t.join(); });
    for_each(m_writerThreads.begin(), m_writerThreads.end(), [](auto& t) {t.join(); });

    m_writerThreads.clear();
    m_readerThreads.clear();
}

