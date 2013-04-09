/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  · Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *  · Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *  · Neither the name of the Ford Motor Company nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * WebSocket core simulation
 * 
 * It is entry point for communication with Android sen and receive methods are
 * used for actual communication with Android native code
 * 
 */
FFW.WebSocketSimulator = Em.Object.create( {

    /*
     * send message via JavaScriptFacade interface of Android
     */
    send: function( clientName, jsonMessage ) {
        if( FFW.isAndroid ){
            Android.send( clientName, jsonMessage );
        }
    },

    /*
     * receive message from Android to be called this way:
     * FFW.WebSocketSimulator.receive(<RPCComponentName>, <jsonMessage>)
     */
    receive: function( clientName, jsonMessage ) {
        var index = this.findByName( clientName );
        if( index != -1 ){
            this.clients[index].onmessage( {
                data: jsonMessage
            } );
        }
    },

    /*
     * add "WebSocket" to the pull later it will be used for receiving of
     * messages from Android
     */
    addClient: function( client ) {
        this.clients.push( client );
    },

    /*
     * remove "WebSocket" from the pull messages for specified client won't be
     * observed anymore
     */
    removeClient: function( client ) {
        var index = this.findByName( client.clientName );
        if( index != -1 ){
            this.clients.splice( index, 1 );
        }
    },

    findByName: function( name ) {
        for( var i = 0; i < this.clients.length; i++ )
            if( this.clients[i].clientName == name )
                return i;

        return -1;
    },

    /*
     * pull of "Websocket"s sending and receiving messages
     */
    clients: []
} );
