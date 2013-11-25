/*
 * @name ReverseSDL
 *
 *
 * @category	RPC component
 * @filesource	web/ReverseSDL.json
 * @version	1.0
 *
 * @author	Sergei Polishchuk
 *
 */
RevSDL = {
    methods: {
        /**
         * Sends a request for access to the management of head unit, through SDL interface
         **/
        GrantAccess: {

        },

        /**
         * Sends a request for cancel access to the management of head unit, through SDL interface
         **/
        CancelAccess: {

        },

        /**
         * Sends a request to change specified parameter for radio on head unit, through SDL interface
         **/
        TuneRadio: {
            parameters: {
                RadioStation: {
                    frequency: "int",
                    fraction: "int",
                    HD: "int"
                }
            }
        },

        /**
         * Start frequency scan on head unit, through SDL interface
         **/
        StartScan: {

        },

        /**
         * Stop frequency scan on head unit, through SDL interface
         **/
        StopScan: {

        },

        /** Sends a request show application on HMI, through SDL interface**/
        Show: {

        }
    },

    notifications: {
        OnControlChanged:{
            parameters:{
                /**
                 * SDL interrupt access status
                 * 1. 'DRIVER_FOCUS'
                 * 2. 'TIME_OUT'
                 */
                reason: "string"
            }
        },

        OnRadioDetails: {
            parameters: {
                radioStation: {
                    frequency: "int",
                    fraction: "int",
                    HD: "int"
                },
                songInfo: {
                    name: "string",
                    artist: "string",
                    genre: "string"
                }
            }
        }
    }
};