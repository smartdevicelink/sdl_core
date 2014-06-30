/*
 * This temporary stub
 */
.pragma library
.import "../hmi_api/Common.js" as Common

function SDL_UpdateSDL(callback) {
    callback(Common.UpdateResult.UP_TO_DATE);
}

function SDL_GetURLS(service, callback) {
    var urls = service === 7 ?
                [{url: "http://policies.telematics.ford.com/api/policies"}] :
                [];
    callback(urls);
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

function SDL_GetStatusUpdate(callback) {
    callback(Common.UpdateResult.UPDATE_NEEDED);
}
