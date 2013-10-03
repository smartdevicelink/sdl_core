import QtQuick 2.0

Item {
    function updateDeviceList (deviceList) {
        dataContainer.deviceList.clear();
        for(var i = 0; i < deviceList.length; i++) {
            dataContainer.deviceList.append({ name: deviceList[i].name, devid: deviceList[i].id })
        }
    }

    function updateAppList (applications) {
        dataContainer.applicationList.clear();
        for(var i = 0; i < applications.length; i++) {
            dataContainer.applicationList.append({
                 appName: applications[i].appName,
                 ngnMediaScreenAppName: applications[i].ngnMediaScreenAppName,
                 icon: applications[i].icon,
                 deviceName: applications[i].deviceName,
                 appId: applications[i].appID,
                 hmiDisplayLanguageDesired: applications[i].hmiDisplayLanguageDesired,
                 isMediaApplication: applications[i].isMediaApplication,
                 appType: applications[i].appType
            });
        }
    }

    function allowDeviceToConnect (device) {
        return {
            allow: true
        }
    }
}
