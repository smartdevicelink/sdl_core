import QtQuick 2.0

Item
{
    property var capabilities: []

    function getCapabilities(test)
    {
        console.log(capabilities)
        return {
            buttonCapabilities: capabilities
            /*[
                {
                    name: 100,
                    shortPressAvailable: true,
                    longPressAvailable: false,
                    upDownAvailable: true
                },
                {
                    name: 2,
                    shortPressAvailable: false,
                    longPressAvailable: true,
                    upDownAvailable: true
                }
            ]*/,
            presetBankCapabilities: {
                onScreenPresetsAvailable: test.testItem
            }
        };
    }
}
