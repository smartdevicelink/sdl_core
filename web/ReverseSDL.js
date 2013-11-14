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
        sendGrantAccessRequest: {

        },

        /**
         * Sends a request for cancel access to the management of head unit, through SDL interface
         **/
        sendCancelAccessRequest: {

        },

        /**
         * Sends a request to change specified parameter for radio on head unit, through SDL interface
         **/
        sendTuneRadioRequest: {
            parameters: {
                RadioStation: {
                    frequency: "int",
                    fraction: "int",
                    HD: "int"
                }
            }
        }
    },

    notifications: {
        onAccessChanged:{
            parameters:{
                /**
                 * SDL access status
                 * 1. 'granted'
                 * 2. 'denied'
                 */
                status: "string"
            }
        },

        onRadioDetails: {
            parameters: {
                RadioStation: {
                    frequency: "int",
                    fraction: "int",
                    HD: "int"
                },
                SongInfo: {
                    name: "string",
                    artist: "string",
                    genre: "string"
                }
            }
        }
    }
};