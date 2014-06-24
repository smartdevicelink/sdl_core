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
