/*
 * This temporary stub
 */
.pragma library
.import "../hmi_api/Common.js" as Common

function SDL_UpdateSDL(callback) {
    console.log("enter");
    callback(Common.UpdateResult.UP_TO_DATE);
    console.log("exit");
}

function SDL_ActivateApp(appId, callback) {
    console.log("SDL_ActivateApp enter");
    callback({
                 "isAppPermissionsRevoked":false,
                 "isAppRevoked":false,
                 "isPermissionsConsentNeeded":false,
                 "isSDLAllowed":false,
                 "device":{
                     "name":"10.10.0.1",
                     "id":'1'
                 }
             });
    console.log("SDL_ActivateApp exit");
}
