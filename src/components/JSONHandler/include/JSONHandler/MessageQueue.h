/**
* \file MessageQueue.hpp
* \brief Template MessageQueue class header.
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS 


#include <pthread.h>
#include <queue>

/**
 * \class MessageQueue
 * \brief Wrapper for multithreading queue.
*/
template <typename T> class MessageQueue
{
public:
    /**
     * \brief Default constructor
    */
    MessageQueue();

    /**
     * \brief Constructor
     * \param queue Existing queue.
    */
    explicit MessageQueue( std::queue<T> queue );

    /**
     * \brief Destructor
    */
    ~MessageQueue();

    /**
     * \brief Returns size of the queue.
     * \return Size of the queue.
     */
    int size() const;

    /**
     * \brief If queue is empty.
     * \return Is queue empty.
     */
    bool empty() const;

    /**
     * \brief Adds element to the queue.
     * \param element Element to be added to the queue.n
     */
    void push( const T & element );

    /**
     * \brief Removes element from the queue and returns it.
     * \return To element of the queue.
     */
    T pop();

    /**
     * \brief Conditional wait.
     */
    void wait();

private:
    /**
      *\brief Queue
    */
    std::queue<T> mQueue;
    /**
      *\brief Mutex for queue locking.
    */
    mutable pthread_mutex_t mMutex;
    /**
      *\brief Condition for waiting.
    */
    pthread_cond_t mCond;
    /**
      *\brief Bool condition for waiting.
    */
    bool mIsUp;
};

template <typename T> MessageQueue<T>::MessageQueue()
:mMutex( PTHREAD_MUTEX_INITIALIZER )
,mCond( PTHREAD_COND_INITIALIZER )
,mIsUp( false )
{
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
    pthread_mutex_lock( &mMutex );
    mQueue.push( element );

    pthread_cond_signal( &mCond );
    mIsUp = true;

    pthread_mutex_unlock( &mMutex );
}

template <typename T> T MessageQueue<T>::pop( )
{
    pthread_mutex_lock( &mMutex );
    if ( mQueue.empty() )
    {
        //error, TRACE
    }
    
    T result = mQueue.front();
    mQueue.pop();
       
    pthread_mutex_unlock( &mMutex );
    return result; 
}

#endif  //  MESSAGE_QUEUE_CLASS
