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
 * @name SDL.TableLabel
 * @desc Universal TableLabel component for SDL application
 * @category Controlls
 * @filesource app/controlls/TableLabel.js
 * @version 1.0
 */

SDL.TableLabel = Em.View.extend( {
    classNameBindings:
        [
            'disabled'
        ],
    /** Define button template */
    template: Ember.Handlebars.compile( 
                    '{{#with view}}' + '{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'
                    + '<span class="content">  {{content}} </span>'
                    + '{{#if rightIcon}}<img class="right-icon" {{bindAttr src="rightIcon"}} />{{/if}}'
                    + '{{#if contentSecond}} <span class="content-second">  {{contentSecond}} </span> {{/if}}'
                    + '{{#if contentThird}}  <span class="content-third">   {{contentThird}}  </span> {{/if}}' + '{{/with}}' )
} );