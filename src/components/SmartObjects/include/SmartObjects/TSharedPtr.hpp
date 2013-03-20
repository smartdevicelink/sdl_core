#ifndef __TSHAREDPTR_HPP__
#define __TSHAREDPTR_HPP__

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Shared pointer.
         *
         * Pointer to an object with reference counting.
         * Object will be automatically deallocated when last shared
         * pointer is destroyed.
         *
         * @tparam ObjectType Type of wrapped object.
         **/
        template <typename ObjectType>
        class TSharedPtr
        {
        public:
            /**
             * @brief Constructor.
             *
             * Initialize shared pointer with wrapped object.
             * Reference counter will be initialized to 1.
             *
             * @param Object Wrapped object.
             **/
            TSharedPtr(ObjectType * Object);

            /**
             * @brief Copy constructor.
             *
             * Initialize shared pointer with another shared pointer.
             * Reference counter will be incremented.
             *
             * @param Other Other shared pointer.
             **/
            TSharedPtr(const TSharedPtr<ObjectType> & Other);

            /**
             * @brief Copy constructor.
             *
             * Initialize shared pointer with another shared pointer.
             * Reference counter will be incremented.
             *
             * @tparam OtherObjectType Type of other object pointer. This
             *                         allows creating a shared pointer to an
             *                         intstance of a base class from a shared
             *                         pointer to an instance of a class
             *                         inherited from this base class.
             *                         If OtherObjectType is not implicitly
             *                         convertible to ObjectType it will
             *                         cause a compile error.
             *
             * @param Other Other shared pointer.
             **/
            template <typename OtherObjectType>
            TSharedPtr(const TSharedPtr<OtherObjectType> & Other);

            /**
             * @brief Destructor.
             *
             * Decrement reference counter and destroy wrapped object
             * if reference counter reaches zero.
             **/
            ~TSharedPtr(void);

            /**
             * @brief Assignment operator.
             *
             * Drop reference to currently referenced object and add
             * reference to assigned object.
             *
             * @param Other Shared pointer to an object
             *              that must be referenced.
             *
             * @return Reference to this shared pointer.
             **/
            TSharedPtr<ObjectType> & operator =(const TSharedPtr<ObjectType> & Other);

            /**
             * @brief Assignment operator.
             *
             * Drop reference to currently referenced object and add
             * reference to assigned object.
             *
             * @tparam OtherObjectType Type of other object pointer. This
             *                         allows creating a shared pointer to an
             *                         intstance of a base class from a shared
             *                         pointer to an instance of a class
             *                         inherited from this base class.
             *                         If OtherObjectType is not implicitly
             *                         convertible to ObjectType it will
             *                         cause a compile error.
             *
             * @param Other Shared pointer to an object
             *              that must be referenced.
             *
             * @return Reference to this shared pointer.
             **/
            template <typename OtherObjectType>
            TSharedPtr<ObjectType> & operator =(const TSharedPtr<OtherObjectType> & Other);

            /**
             * @brief Member access operator.
             *
             * @return Wrapped object.
             **/
            ObjectType * operator->(void) const;

        private:
            // TSharedPtr needs access to other TSharedPtr private members for shared pointers type casts.
            template <typename OtherObjectType>
            friend class TSharedPtr;

            /**
             * @brief Drop reference to wrapped object.
             *
             * If reference counter reaches zero object and its reference
             * counter will be deallocated.
             **/
            void dropReference(void);

            /**
             * @brief Wrapped object.
             **/
            ObjectType * mObject;

            /**
             * @brief Pointer to reference counter.
             **/
            unsigned int * mReferenceCounter;
        };
    }
}

template <typename ObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::TSharedPtr(ObjectType * Object):
mObject(Object),
mReferenceCounter(new unsigned int(1))
{
}

template <typename ObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::TSharedPtr(const TSharedPtr<ObjectType> & Other):
mObject(0),
mReferenceCounter(0)
{
    *this = Other;
}

template <typename ObjectType>
template <typename OtherObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::TSharedPtr(const TSharedPtr<OtherObjectType> & Other):
mObject(0),
mReferenceCounter(0)
{
    *this = Other;
}

template <typename ObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::~TSharedPtr(void)
{
    dropReference();
}

template <typename ObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType> & NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::operator=(const TSharedPtr<ObjectType> & Other)
{
    return operator=<ObjectType>(Other);
}

template <typename ObjectType>
template <typename OtherObjectType>
inline NsAppLink::NsSmartObjects::TSharedPtr<ObjectType> & NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::operator=(const TSharedPtr<OtherObjectType> & Other)
{
    dropReference();

    mObject = Other.mObject;
    mReferenceCounter = Other.mReferenceCounter;

    if (0 != mReferenceCounter)
    {
        ++*mReferenceCounter;
    }

    return *this;
}

template <typename ObjectType>
ObjectType * NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::operator->(void) const
{
    return mObject;
}

template <typename ObjectType>
inline void NsAppLink::NsSmartObjects::TSharedPtr<ObjectType>::dropReference(void)
{
    if (0 != mReferenceCounter)
    {
        if (0 == --*mReferenceCounter)
        {
            delete mObject;
            mObject = 0;

            delete mReferenceCounter;
            mReferenceCounter = 0;
        }
    }
}

#endif
