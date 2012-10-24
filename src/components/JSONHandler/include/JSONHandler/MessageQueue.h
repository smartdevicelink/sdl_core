#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS value

#include <queue>
#include <pthread.h>

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
    void wait();

private:
    /* data */
    std::queue<T> mQueue;
    //mutable sem_t mMutex;
    mutable pthread_mutex_t mMutex;
    pthread_cond_t mCond;
    bool mIsUp;
};

template <typename T> MessageQueue<T>::MessageQueue()
:mMutex( PTHREAD_MUTEX_INITIALIZER )
,mCond( PTHREAD_COND_INITIALIZER )
,mIsUp( false )
{
    //sem_init( &mMutex, 0, 1 );
    pthread_mutex_init( &mMutex, NULL );
    pthread_cond_init( &mCond, NULL );
}

template <typename T> MessageQueue<T>::MessageQueue( std::queue<T> queue )
{
    pthread_mutex_init( &mMutex, NULL );
    pthread_cond_init( &mCond, NULL );
    pthread_mutex_lock( &mMutex );
    mQueue = std::queue<T>( queue );
    pthread_mutex_unlock( &mMutex );
}

template <typename T> MessageQueue<T>::~MessageQueue()
{
   /* sem_wait( &mMutex );
    while ( !mQueue.empty() ) 
    {
        mQueue.pop();
    }
    sem_post( &mMutex );*/

    //sem_destroy( &mMutex );
    pthread_cond_destroy( &mCond );
    pthread_mutex_destroy( &mMutex );
}

template<typename T> void MessageQueue<T>::wait()
{
    pthread_mutex_lock( &mMutex );
    while ( !mIsUp ) 
    {
        pthread_cond_wait( &mCond, &mMutex );
    }
    mIsUp = false;
    pthread_mutex_unlock( &mMutex );
}

template <typename T> int MessageQueue<T>::size() const
{
    int result = 0;
    pthread_mutex_lock( &mMutex );
    result = mQueue.size();
    pthread_mutex_unlock( &mMutex );
    return result;
}
    
template <typename T> bool MessageQueue<T>::empty() const
{
    bool result = true;
    pthread_mutex_lock( &mMutex );
    result = mQueue.empty();
    pthread_mutex_unlock( &mMutex );
    return result;
}

template <typename T> void MessageQueue<T>::push( const T & element )
{
    //sem_wait( &mMutex );
    pthread_mutex_lock( &mMutex );
    mQueue.push( element );

    pthread_cond_signal( &mCond );
    mIsUp = true;

    //sem_post( &mMutex );
    pthread_mutex_unlock( &mMutex );
}

template <typename T> T MessageQueue<T>::pop( )
{
    //sem_wait( &mMutex );
    pthread_mutex_lock( &mMutex );
    if ( mQueue.empty() )
    {
        //error, TRACE
        //sem_post( &mMutex );
    }
    
    T result = mQueue.front();
    mQueue.pop();
       
    //sem_post( &mMutex );
    pthread_mutex_unlock( &mMutex );
    return result; 
}

#endif