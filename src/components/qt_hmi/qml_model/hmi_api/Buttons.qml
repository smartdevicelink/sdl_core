import QtQuick 2.0

Item
{
    property var capabilities: []

    signal onButtonEvent(int name, int mode, var customButtonId)
    signal onButtonPress(int name, int mode, var customButtonId)

    function getCapabilities(test)
    {
        console.log(capabilities)
        return {
            buttonCapabilities: capabilities,
            presetBankCapabilities: {
                onScreenPresetsAvailable: test.testItem
            }
        };
    }

}
