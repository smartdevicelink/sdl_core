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
		
	elementId:		'info_calendar',
	
	template: Ember.Handlebars.compile(
		'<div class="inner-wrapper">'+
				'<div class="label-20 disabled date"><span>{{MFT.locale.label.view_info_calendar_date}}</span></div>'+

				'{{view MFT.Button '+
					'class= "today button" '+
					'textBinding="MFT.locale.label.view_info_calendar_today" '+
					'disabled=true '+
				'}}'+

				'<div class="day day-1"><span>{{MFT.locale.label.view_info_calendar_day1}}</span></div>'+
				'<div class="day day-2"><span>{{MFT.locale.label.view_info_calendar_day2}}</span></div>'+
				'<div class="day day-3"><span>{{MFT.locale.label.view_info_calendar_day3}}</span></div>'+
				'<div class="day day-4"><span>{{MFT.locale.label.view_info_calendar_day4}}</span></div>'+
				'<div class="day day-5"><span>{{MFT.locale.label.view_info_calendar_day5}}</span></div>'+
				'<div class="day day-6"><span>{{MFT.locale.label.view_info_calendar_day6}}</span></div>'+
				'<div class="day day-7 active"><span>{{MFT.locale.label.view_info_calendar_day7}}</span></div>'+

				'<div class="c-btn disabled btn-1"><span>1</span></div>'+
				'<div class="c-btn disabled btn-8"><span>8</span></div>'+
				'<div class="c-btn disabled btn-15"><span>15</span></div>'+
				'<div class="c-btn disabled btn-22"><span>22</span></div>'+
				'<div class="c-btn disabled btn-29"><span>29</span></div>'+

				'<div class="c-btn disabled btn-7"><span>7</span></div>'+
				'<div class="c-btn disabled btn-14"><span>14</span></div>'+
				'<div class="c-btn disabled btn-21"><span>21</span></div>'+
				'<div class="c-btn disabled active btn-28"><span>28</span></div>'+

				'<div class="c-btn disabled btn-6"><span>6</span></div>'+
				'<div class="c-btn disabled btn-13"><span>13</span></div>'+
				'<div class="c-btn disabled btn-20"><span>20</span></div>'+
				'<div class="c-btn disabled btn-27"><span>27</span></div>'+

				'<div class="c-btn disabled btn-5"><span>5</span></div>'+
				'<div class="c-btn disabled btn-12"><span>12</span></div>'+
				'<div class="c-btn disabled btn-19"><span>19</span></div>'+
				'<div class="c-btn disabled btn-26"><span>26</span></div>'+

				'<div class="c-btn disabled btn-4"><span>4</span></div>'+
				'<div class="c-btn disabled btn-11"><span>11</span></div>'+
				'<div class="c-btn disabled btn-18"><span>18</span></div>'+
				'<div class="c-btn disabled btn-25"><span>25</span></div>'+

				'<div class="c-btn disabled btn-3"><span>3</span></div>'+
				'<div class="c-btn disabled btn-10"><span>10</span></div>'+
				'<div class="c-btn disabled btn-17"><span>17</span></div>'+
				'<div class="c-btn disabled btn-24"><span>24</span></div>'+
				'<div class="c-btn disabled btn-31"><span>31</span></div>'+

				'<div class="c-btn disabled btn-2"><span>2</span></div>'+
				'<div class="c-btn disabled btn-9"><span>9</span></div>'+
				'<div class="c-btn disabled btn-16"><span>16</span></div>'+
				'<div class="c-btn disabled btn-23"><span>23</span></div>'+
				'<div class="c-btn disabled btn-30"><span>30</span></div>'+

			'</div>'
	)	

});