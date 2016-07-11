/*#include <stdint.h>
#include <string>
#include <set>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "utils/custom_string.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/application_impl.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "application_manager/mock_application_manager.h"


namespace test {
namespace components {
namespace commands_test {

using ::utils::SharedPtr;
namespace strings = ::application_manager::strings;
using ::application_manager::commands::CommandImpl;
using ::application_manager::ApplicationManager;
using ::application_manager::commands::MessageSharedPtr;
using ::application_manager::ApplicationSharedPtr;

using ::test::components::application_manager_test::MockApplication;
using usage_statistics_test::MockStatisticsManager;

//Generate random message for testing
static const char alphanum[] =
                            "0123456789"
                            "!@#$%^&*"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
int stringLength = sizeof(alphanum) - 1;

std::string genRandomMessage()
{
    std::string randMessage;
    char letter =  alphanum[rand() % stringLength];
    for(unsigned int i = 0; i < 10; ++i)
       {
       randMessage = letter+ genRandomMessage();
       }
    return randMessage;

}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace custom_str = utils::custom_string;
namespace am = application_manager;

class AddCommandResponseImplTest
    : public CommandResponsetTest<CommandsTestMocks::kIsNice> {
 public:
    class UnwrappedAddCommandResponse: public application_manager::commands::AddCommandResponse {
    public:
        UnwrappedAddCommandResponse(const MessageSharedPtr& message,
                                   ApplicationManager& application_manager)
                : AddCommandResponse(message, application_manager) {

        }
    };
protected:
    sync_primitives::Lock map_lock_;
};

typedef SharedPtr<AddCommandResponseImplTest::UnwrappedAddCommandResponse> AddCommandImplPtr;

TEST_F(AddCommandResponseImplTest, CheckIfCommandServiceIsCorrect){


}

}
}
}
*/
