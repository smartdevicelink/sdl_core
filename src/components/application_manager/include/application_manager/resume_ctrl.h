#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

#include <cstdint>
#include <vector>
#include <map>
#include <set>

#include "json/json.h"
#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "application_manager/application_impl.h"

namespace application_manager {

class ApplicationManagerImpl;
class ResumeCtrl {
  public:
    ResumeCtrl(ApplicationManagerImpl* application_manager);
    ~ResumeCtrl();
    /*
     * @brief Save all applications info to the file system
     */
    void SaveAllApplications() ;

    /*
     * @brief Save application persistent info for future resuming
     * @param application is applicatint whitch need to be saved
     * In case of IGN_OFF or Ctl-C or MEATER_RESSET this info will saveto to file system
     */
    void SaveApplication(Application *application);
    /*
     * @brief Load unregistered applications info from the file system
     */
    void LoadApplications();

    /*
     * @brief Add to application saved persistent data (if exist)
     * @param application is applicatint whitch files need to be restored
     * @param only_persistent (true by default) describes: restore only
     *        persistent files or temporary too
     * @return true if succes,  otherwise return false
     */
    bool RestoreApplicationFiles(Application *application, bool only_persistent = true);

    /*
     * @brief Set application HMI Level as saved
     * @param application is applicatint whitch HMI Level is need to restore
     * @return true if succes, otherwise return false
     */
    bool RestoreApplicationHMILevel(Application *application);
  private:
    static log4cxx::LoggerPtr logger_;
    std::vector<Json::Value> saved_applications_vector;
    ApplicationManagerImpl *application_manager_;
    std::string GetMacAddress(Application *application);
};

}  // namespace application_manager
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H
