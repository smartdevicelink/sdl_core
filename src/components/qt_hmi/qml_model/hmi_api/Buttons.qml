import QtQuick 2.0

Item
{
    function getCapabilities()
    {
        return {
            capabilities: settingsContainer.capabilities,
            presetBankCapabilities: {
                onScreenPresetsAvailable: true
            }
        }
    }
}
