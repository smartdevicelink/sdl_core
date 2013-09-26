import QtQuick 2.0

Item {
    function isReady () {
        return {
            available: settings.vehileInfoAvailable
        }
    }
}
