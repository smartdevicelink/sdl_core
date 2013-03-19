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
             * @brief Type conversion operator.
             *
             * Convert shared pointer to a shared pointer to another object type.
             * This allows creating a shared pointer to an intstance of a base
             * class from a shared pointer to an instance of a class inherited
             * from this base class.
             *
             * If ObjectType is not implicitly convertible to OtherObjectType
             * it will cause a compile error.
             *
             * @tparam OtherObjectType Type of other object pointer.
             *
             * @return Shared pointer to OtherObjectType.
             **/
            template <typename OtherObjectType>
            operator TSharedPtr<OtherObjectType>(void);

            /**
             * @brief Member access operator.
             *
             * @return Wrapped object.
             **/
            ObjectType * operator->(void) const;
        };
    }
}

#endif
