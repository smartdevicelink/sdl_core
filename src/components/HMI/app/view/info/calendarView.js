/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: ·
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. · Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. · Neither the name of the Ford Motor Company nor the
 * names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 * @name SDL.InfoCalendarView
 * @desc Info Calendar module visual representation
 * @category View
 * @filesource app/view/sinfo/CalendarView.js
 * @version 1.0
 */

SDL.InfoCalendarView = Em.View
    .create( {
        /** Bind class for visual representation */
        classNameBindings: [
            'SDL.States.info.calendar.active:active_state:inactive_state'
        ],

        elementId: 'info_calendar',

        template: Ember.Handlebars
            .compile('<div class="inner-wrapper">'
                + '<div class="label-20 disabled date">{{SDL.locale.label.view_info_calendar_date}}</div>'
                +

                '{{view SDL.Button '
                + 'class= "today button" '
                + 'textBinding="SDL.locale.label.view_info_calendar_today" '
                + 'disabled=true '
                + '}}'
                +

                '<div class="day day-1">{{SDL.locale.label.view_info_calendar_day1}}</div>'
                + '<div class="day day-2">{{SDL.locale.label.view_info_calendar_day2}}</div>'
                + '<div class="day day-3">{{SDL.locale.label.view_info_calendar_day3}}</div>'
                + '<div class="day day-4">{{SDL.locale.label.view_info_calendar_day4}}</div>'
                + '<div class="day day-5">{{SDL.locale.label.view_info_calendar_day5}}</div>'
                + '<div class="day day-6">{{SDL.locale.label.view_info_calendar_day6}}</div>'
                + '<div class="day day-7 active">{{SDL.locale.label.view_info_calendar_day7}}</div>'
                +

                '<div class="c-btn disabled btn-1">1</div>'
                + '<div class="c-btn disabled btn-8">8</div>'
                + '<div class="c-btn disabled btn-15">15</div>'
                + '<div class="c-btn disabled btn-22">22</div>'
                + '<div class="c-btn disabled btn-29">29</div>' +

                '<div class="c-btn disabled btn-7">7</div>'
                + '<div class="c-btn disabled btn-14">14</div>'
                + '<div class="c-btn disabled btn-21">21</div>'
                + '<div class="c-btn disabled active btn-28">28</div>' +

                '<div class="c-btn disabled btn-6">6</div>'
                + '<div class="c-btn disabled btn-13">13</div>'
                + '<div class="c-btn disabled btn-20">20</div>'
                + '<div class="c-btn disabled btn-27">27</div>' +

                '<div class="c-btn disabled btn-5">5</div>'
                + '<div class="c-btn disabled btn-12">12</div>'
                + '<div class="c-btn disabled btn-19">19</div>'
                + '<div class="c-btn disabled btn-26">26</div>' +

                '<div class="c-btn disabled btn-4">4</div>'
                + '<div class="c-btn disabled btn-11">11</div>'
                + '<div class="c-btn disabled btn-18">18</div>'
                + '<div class="c-btn disabled btn-25">25</div>' +

                '<div class="c-btn disabled btn-3">3</div>'
                + '<div class="c-btn disabled btn-10">10</div>'
                + '<div class="c-btn disabled btn-17">17</div>'
                + '<div class="c-btn disabled btn-24">24</div>'
                + '<div class="c-btn disabled btn-31">31</div>' +

                '<div class="c-btn disabled btn-2">2</div>'
                + '<div class="c-btn disabled btn-9">9</div>'
                + '<div class="c-btn disabled btn-16">16</div>'
                + '<div class="c-btn disabled btn-23">23</div>'
                + '<div class="c-btn disabled btn-30">30</div>' +

                '</div>')

    });