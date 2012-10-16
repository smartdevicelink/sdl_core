#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS value

#include <queue>
#include <semaphore.h>

template <typename T> class MessageQueue
{
public:
    MessageQueue();
    MessageQueue( std::queue<T> queue );
    ~MessageQueue();

    int size() const;
    bool empty() const;

    void push( const T & element );
    T pop();

private:
    /* data */
    std::queue<T> mQueue;
    mutable sem_t mMutex;
};

template <typename T> MessageQueue<T>::MessageQueue()
{
    sem_init( &mMutex, 0, 1 );
}

template <typename T> MessageQueue<T>::MessageQueue( std::queue<T> queue )
{
    sem_init( &mMutex, 0, 1 );
    sem_wait( &mMutex );
    mQueue = std::queue<T>( queue );
    sem_post( &mMutex );
}

template <typename T> MessageQueue<T>::~MessageQueue()
{
    sem_wait( &mMutex );
    while ( !mQueue.empty() ) 
    {
        mQueue.pop();
    }
    sem_post( &mMutex );

    sem_destroy( &mMutex );
}

template <typename T> int MessageQueue<T>::size() const
{
    int result = 0;
    sem_wait( &mMutex );
    result = mQueue.size();
    sem_post( &mMutex );
    return result;
}
    
template <typename T> bool MessageQueue<T>::empty() const
{
    bool result = true;
    sem_wait( &mMutex );
    result = mQueue.empty();
    sem_post( &mMutex );
    return result;
}

template <typename T> void MessageQueue<T>::push( const T & element )
{
    sem_wait( &mMutex );
    mQueue.push( element );
    sem_post( &mMutex );
}

template <typename T> T MessageQueue<T>::pop( )
{
    sem_wait( &mMutex );
    if ( mQueue.empty() )
    {
        //error, TRACE
        //sem_post( &mMutex );
    }
    
    T result = mQueue.front();
    mQueue.pop();
       
    sem_post( &mMutex );
    return result; 
}

#endif