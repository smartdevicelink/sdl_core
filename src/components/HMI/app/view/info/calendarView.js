/**
 * @name MFT.InfoCalendarView
 * 
 * @desc Info Calendar module visual representation
 * 
 * @category	View
 * @filesource	app/view/sinfo/CalendarView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.InfoCalendarView = Em.View.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'info_calendar',
	
	template: Ember.Handlebars.compile(
		'<div class="inner-wrapper">'+
				'<div class="label-20 disabled date">{{MFT.locale.label.view_info_calendar_date}}</div>'+

				'{{view MFT.Button '+
					'class= "today button" '+
					'textBinding="MFT.locale.label.view_info_calendar_today" '+
					'disabled=true '+
				'}}'+

				'<div class="day day-1">{{MFT.locale.label.view_info_calendar_day1}}</div>'+
				'<div class="day day-2">{{MFT.locale.label.view_info_calendar_day2}}</div>'+
				'<div class="day day-3">{{MFT.locale.label.view_info_calendar_day3}}</div>'+
				'<div class="day day-4">{{MFT.locale.label.view_info_calendar_day4}}</div>'+
				'<div class="day day-5">{{MFT.locale.label.view_info_calendar_day5}}</div>'+
				'<div class="day day-6">{{MFT.locale.label.view_info_calendar_day6}}</div>'+
				'<div class="day day-7 active">{{MFT.locale.label.view_info_calendar_day7}}</div>'+

				'<div class="c-btn disabled btn-1">1</div>'+
				'<div class="c-btn disabled btn-8">8</div>'+
				'<div class="c-btn disabled btn-15">15</div>'+
				'<div class="c-btn disabled btn-22">22</div>'+
				'<div class="c-btn disabled btn-29">29</div>'+

				'<div class="c-btn disabled btn-7">7</div>'+
				'<div class="c-btn disabled btn-14">14</div>'+
				'<div class="c-btn disabled btn-21">21</div>'+
				'<div class="c-btn disabled active btn-28">28</div>'+

				'<div class="c-btn disabled btn-6">6</div>'+
				'<div class="c-btn disabled btn-13">13</div>'+
				'<div class="c-btn disabled btn-20">20</div>'+
				'<div class="c-btn disabled btn-27">27</div>'+

				'<div class="c-btn disabled btn-5">5</div>'+
				'<div class="c-btn disabled btn-12">12</div>'+
				'<div class="c-btn disabled btn-19">19</div>'+
				'<div class="c-btn disabled btn-26">26</div>'+

				'<div class="c-btn disabled btn-4">4</div>'+
				'<div class="c-btn disabled btn-11">11</div>'+
				'<div class="c-btn disabled btn-18">18</div>'+
				'<div class="c-btn disabled btn-25">25</div>'+

				'<div class="c-btn disabled btn-3">3</div>'+
				'<div class="c-btn disabled btn-10">10</div>'+
				'<div class="c-btn disabled btn-17">17</div>'+
				'<div class="c-btn disabled btn-24">24</div>'+
				'<div class="c-btn disabled btn-31">31</div>'+

				'<div class="c-btn disabled btn-2">2</div>'+
				'<div class="c-btn disabled btn-9">9</div>'+
				'<div class="c-btn disabled btn-16">16</div>'+
				'<div class="c-btn disabled btn-23">23</div>'+
				'<div class="c-btn disabled btn-30">30</div>'+

			'</div>'
	)	

});