import QtQuick 2.0

Item {
    signal onReady;
    signal onStartDeviceDiscovery;

    function updateDeviceList (deviceList) {
    }

    function allowDeviceToConnect (device) {
        return {
            allow: true
        }
    }
}
