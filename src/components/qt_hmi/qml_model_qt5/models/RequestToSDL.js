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
                 "isPermissionsConsentNeeded":true,
                 "isSDLAllowed":true,
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

function SDL_GetListOfPermissions(appId, callback) {
    console.log("SDL_GetListOfPermissions enter");
    callback([
                {
                    "name":"Notifications",
                    "id":0,
                    "allowed":false,
                },
                {
                    "name":"DrivingCharacteristics",
                    "id":1,
                    "allowed":true,
                },
                {
                    "name":"VehicleInfo",
                    "id":2,
                    "allowed":false,
                },
                {
                    "name":"Location",
                    "id":3,
                    "allowed":true,
                }
             ]);
    console.log("SDL_GetListOfPermissions exit");
}

function SDL_GetUserFriendlyMessage(messageCodes, Language, callback){
    console.log("SDL_GetUserFriendlyMessage enter");

    callback([{
                  "messageCode": "VehicleInfo",
                  "tts": "An app can access the following vehicle information: Fuel Level, Fuel Economy, Engine RPMs, Odometer, VIN, External Temperature, Gear Position, Tire Pressure.",
                  "label": "Vehicle information",
                  "textBody": "An app can access the following vehicle information: Fuel Level, Fuel Economy, Engine RPMs, Odometer, VIN, External Temperature, Gear Position, Tire Pressure."
              },
              {
                  "messageCode": "Notifications",
                  "tts": "An app can send notifications when running in the background.",
                  "label": "Push notifications",
                  "textBody": "An app can send notifications when running in the background."
              },
              {
                  "messageCode": "Location",
                  "tts": "An app can access vehicle GPS and speed.",
                  "label": "GPS and speed",
                  "textBody": "An app can access vehicle GPS and speed."
              },
              {
                  "messageCode": "DrivingCharacteristics",
                  "tts": "An app can access the following driving characteristics: Fuel Consumption, MyKey, Seat Belt Status.",
                  "label": "Driving Characteristics",
                  "textBody": "An app can access the following driving characteristics: Fuel Consumption, MyKey, Seat Belt Status."
              }
             ])

    console.log("SDL_GetUserFriendlyMessage exit");
}
