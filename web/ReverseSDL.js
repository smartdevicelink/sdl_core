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
         * Sends a request for access to the management of HMI, through SDL interface
         **/
        sendGrantAccessRequest: {

        },

        /**
         * Sends a request for cancel access to the management of HMI, through SDL interface
         **/
        sendCancelAccessRequest: {

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

        onDirectTune: {
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