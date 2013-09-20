import QtQuick 2.0

Item {
    signal onReady;
    signal onStartDeviceDiscovery;

    function fireOnReady() {
        onReady();
    }

    function fireOnStartDeviceDiscovery() {
        onStartDeviceDiscovery();
    }

    function updateDeviceList(deviceList) {
    }
}
