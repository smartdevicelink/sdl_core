import QtQuick 2.0

Item
{
    function getCapabilities()
    {
        return {
            capabilities: settings.capabilities,
            presetBankCapabilities: {
                onScreenPresetsAvailable: true
            }
        }
    }
}
