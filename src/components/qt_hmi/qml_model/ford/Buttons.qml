import QtQuick 2.0

Item {
    function getCapabilities()
    {
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

    function hello(gg)
    {
        console.log("hello called with gg = " + gg)
        gg = 16;
        return [ { name: "Vasya", id: 10 }, 12 ];
    }
}
