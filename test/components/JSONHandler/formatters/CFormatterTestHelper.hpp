#ifndef __CFORMATTERTESTHELPER_HPP__
#define __CFORMATTERTESTHELPER_HPP__

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"

namespace test { namespace components { namespace JSONHandler { namespace formatters {


    class CFormatterTestHelper : public ::testing::Test
    {
    protected:
        void fillTestObject(NsAppLink::NsSmartObjects::CSmartObject& obj);

        //bool checkTestObject(const NsAppLink::NsSmartObjects::CSmartObject& obj);

        void compareObjects(const NsAppLink::NsSmartObjects::CSmartObject& first,
                            const NsAppLink::NsSmartObjects::CSmartObject& second);
    };

}}}}

#endif // __CFORMATTERTESTHELPER_HPP__
