/**
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

#ifndef MULTITHREADED_MAP_CLASS
#define MULTITHREADED_MAP_CLASS

// TODO(AK): Are we use it?

#include <map>

template <typename T, typename K> class MultithreadingMap
{
public:
    MultithreadingMap();
    ~MultithreadingMap();

    int size() const;

    bool empty() const;

    void insert( const std::pair<const T,K> & element );

    K & find( const T & key );

private:
    std::multimap<T,K> mMap;

    /**
      *\brief Mutex for queue locking.
    */
    mutable pthread_mutex_t mMutex;

};

template <typename T, typename K> MultithreadingMap<T,K>::MultithreadingMap() :
mMutex( PTHREAD_MUTEX_INITIALIZER )
{
    pthread_mutex_init( &mMutex, NULL );
}

template <typename T, typename K> MultithreadingMap<T,K>::~MultithreadingMap()
{
    pthread_mutex_destroy( &mMutex );
}



#endif // MULTITHREADED_MAP_CLASS
