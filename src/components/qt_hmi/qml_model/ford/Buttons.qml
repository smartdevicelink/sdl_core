import QtQuick 2.0

Item {

    signal notify(string what)

    function getCapabilities()
    {
        notify("hello");

        return {
            buttonCapabilities:
            [
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
            ],
            presetBankCapabilities: {
                onScreenPresetsAvailable: false
            }
        };
    }

    function fireNotify(what)
    {
        notify(what)
    }
}
