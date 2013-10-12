.pragma library

var async_uid = 1;

function AsyncCall() {
    this.__async_uid = async_uid++;
}
