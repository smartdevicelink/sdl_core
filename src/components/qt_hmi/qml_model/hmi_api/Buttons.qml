import QtQuick 2.0

Item
{

    function getCapabilities()
    {
        console.log(settings.capabilities)
        return {
            buttonCapabilities: settings.capabilities,
            presetBankCapabilities: {
                onScreenPresetsAvailable: true
            }
        }
    }
}
