import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item {
    /// HMI context view is related to
    property int systemContext: Common.SystemContext.SYSCTXT_MAIN
    /// True if view is in application context
    property bool applicationContext: false
    /// View category for AppDeactivated notification (reason)
    property int category: Common.DeactivateReason.GENERAL
}
