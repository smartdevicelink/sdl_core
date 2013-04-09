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
/**
 * @name Size
 * @desc Abstract scale class
 * @category model
 * @filesource app/model/abstract/Scale.js
 * @version 1.0
 */
FFW.Scale = Em.Object.create( {
    /** Scale Point */
    scalePoint: 1,

    scale: function() {

        /* ANDROID */
        if( FFW.isAndroid ){
            /** Scale Rate */
            if( FFW.Backend.isFullScreen ){
                /** Calculate Scale Point */
                var scalePointW = ( FFW.Backend.windowSize.width / 800 ) / FFW.Backend.windowDensity;
                var scalePointH = ( FFW.Backend.windowSize.height / 480 ) / FFW.Backend.windowDensity;
                /** Set calculated ScalePoint */
                ( scalePointW >= scalePointH ) ? this.set( 'scalePoint', scalePointH ) : this.set( 'scalePoint', scalePointW )
            }else{
                this.set( 'scalePoint', 1 / FFW.Backend.windowDensity )
            }
            /** Scale Application */
            $( 'body' ).css( {
                '-webkit-transform': 'scale(' + this.scalePoint + ')',
                '-webkit-transform-origin': '0px 0px'
            } );

            /** Left Shift */
            var LeftShiftAppPercents = ( FFW.Backend.windowSize.width - 800 * this.scalePoint * FFW.Backend.windowDensity ) / 2 / FFW.Backend.windowDensity;
            /** Right Shift */
            var UpShiftAppPercents = ( FFW.Backend.windowSize.height - 480 * this.scalePoint * FFW.Backend.windowDensity ) / 2 / FFW.Backend.windowDensity;
            /** Align Application */
            $( 'body' ).css( {
                'margin-left': LeftShiftAppPercents + 'px',
                'margin-top': UpShiftAppPercents + 'px'
            } );

            // send response to BE for video resize
            Em.Logger.log( "FFW.Backend.resizeVideo = " + FFW.Backend.resizeVideo );
            if( FFW.Backend.resizeVideo )
                FFW.Backend.sendFullScreenResponse( $( 'body' ).offset().top, $( 'body' ).offset().left, this.scalePoint )
        }
    }.observes( 'FFW.Backend.isFullScreen', 'FFW.Backend.windowDensity' )
} )
