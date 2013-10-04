import QtQuick 2.0

Item
{
    function getCapabilities()
    {
        return {
            capabilities: settingsContainer.buttonCapabilities,
            presetBankCapabilities: {
                onScreenPresetsAvailable: true
            }
        }
    }
}
