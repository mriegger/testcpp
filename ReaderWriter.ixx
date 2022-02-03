#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>
export module ReaderWriter;

export class ReaderWriter
{
public:

    void Run();

private:

    void SpawnThreads();
    void JoinThreads();

    void ReaderThread(const int id);
    void WriterThread(const int id);

    static const int NumReaderThreads = 2;
    static const int NumWriterThreads = 2;

    std::vector<std::jthread> m_readerThreads, m_writerThreads;
    std::vector<int> m_sharedData;
    std::condition_variable m_readPhase;
    std::condition_variable m_writePhase;
    std::mutex m_counterMutex;
    int m_resourceCounter = 0;
};

