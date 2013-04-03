#ifndef __CFORMATTERTESTHELPER_HPP__
#define __CFORMATTERTESTHELPER_HPP__

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"

namespace test { namespace components { namespace JSONHandler { namespace formatters {


    class CFormatterTestHelper : public ::testing::Test
    {
    protected:
        void fillTestObject(NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

        //bool checkTestObject(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

        void compareObjects(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& first,
                            const NsSmartDeviceLink::NsSmartObjects::CSmartObject& second);
    };

}}}}

#endif // __CFORMATTERTESTHELPER_HPP__
