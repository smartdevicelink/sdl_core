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
