import QtQuick 2.0

Item
{
    property var capabilities: []

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
