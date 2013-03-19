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
             * @brief Member access operator.
             *
             * @return Wrapped object.
             **/
            ObjectType * operator->(void) const;
        };
    }
}

#endif
