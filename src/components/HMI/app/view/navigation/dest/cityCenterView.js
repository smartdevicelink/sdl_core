/**
 * @name MFT.NavigationDestinationCityCenterView
 * 
 * @desc Navigation Destination City Center module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/CityCenterView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationCityCenterView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_cityCenter',
		
	childViews: [
		'backButton',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled city">{{MFT.locale.label.view_navi_destination_streetAddress_city}}</div>'+
			'<div class="label-20 disabled state">{{MFT.locale.label.view_navi_destination_Freeway_state}}</div>'+
			'<div class="label-20 disabled mi">{{MFT.locale.label.view_navi_destination_Freeway_mi}}</div>'+

			'<div class="label-20 disabled n26">26</div>'+

			'<div class="label-20 disabled pointer">I</div>'+

			'<div class="keyboard">'+

				'<div class="label-20 disabled q">{{MFT.locale.label.view_navi_destination_streetAddress_q}}</div>'+
				'<div class="label-20 disabled w">{{MFT.locale.label.view_navi_destination_streetAddress_w}}</div>'+
				'<div class="label-20 disabled e">{{MFT.locale.label.view_navi_destination_streetAddress_e}}</div>'+
				'<div class="label-20 disabled r">{{MFT.locale.label.view_navi_destination_streetAddress_r}}</div>'+
				'<div class="label-20 disabled t">{{MFT.locale.label.view_navi_destination_streetAddress_t}}</div>'+
				'<div class="label-20 disabled y">{{MFT.locale.label.view_navi_destination_streetAddress_y}}</div>'+
				'<div class="label-20 disabled u">{{MFT.locale.label.view_navi_destination_streetAddress_u}}</div>'+
				'<div class="label-20 disabled i">{{MFT.locale.label.view_navi_destination_streetAddress_i}}</div>'+
				'<div class="label-20 disabled o">{{MFT.locale.label.view_navi_destination_streetAddress_o}}</div>'+
				'<div class="label-20 disabled p">{{MFT.locale.label.view_navi_destination_streetAddress_p}}</div>'+
	
				'<div class="label-20 disabled a">{{MFT.locale.label.view_navi_destination_streetAddress_a}}</div>'+
				'<div class="label-20 disabled s">{{MFT.locale.label.view_navi_destination_streetAddress_s}}</div>'+
				'<div class="label-20 disabled d">{{MFT.locale.label.view_navi_destination_streetAddress_d}}</div>'+
				'<div class="label-20 disabled f">{{MFT.locale.label.view_navi_destination_streetAddress_f}}</div>'+
				'<div class="label-20 disabled g">{{MFT.locale.label.view_navi_destination_streetAddress_g}}</div>'+
				'<div class="label-20 disabled h">{{MFT.locale.label.view_navi_destination_streetAddress_h}}</div>'+
				'<div class="label-20 disabled j">{{MFT.locale.label.view_navi_destination_streetAddress_j}}</div>'+
				'<div class="label-20 disabled k">{{MFT.locale.label.view_navi_destination_streetAddress_k}}</div>'+
				'<div class="label-20 disabled l">{{MFT.locale.label.view_navi_destination_streetAddress_l}}</div>'+
	
				'<div class="label-20 disabled z">{{MFT.locale.label.view_navi_destination_streetAddress_z}}</div>'+
				'<div class="label-20 disabled x">{{MFT.locale.label.view_navi_destination_streetAddress_x}}</div>'+
				'<div class="label-20 disabled c">{{MFT.locale.label.view_navi_destination_streetAddress_c}}</div>'+
				'<div class="label-20 disabled v">{{MFT.locale.label.view_navi_destination_streetAddress_v}}</div>'+
				'<div class="label-20 disabled b">{{MFT.locale.label.view_navi_destination_streetAddress_b}}</div>'+
				'<div class="label-20 disabled n">{{MFT.locale.label.view_navi_destination_streetAddress_n}}</div>'+
				'<div class="label-20 disabled m">{{MFT.locale.label.view_navi_destination_streetAddress_m}}</div>'+
	
				'<div class="label-20 disabled dash">-</div>'+
				'<div class="label-20 disabled and">&</div>'+
	
				'<div class="label-20 disabled abc">{{MFT.locale.label.view_navi_destination_streetAddress_abc}}</div>'+
	
				'<div class="label-20 disabled space">{{MFT.locale.label.view_navi_destination_streetAddress_space}}</div>'+
				'<div class="label-20 disabled ch">{{MFT.locale.label.view_navi_destination_streetAddress_ch}}</div>'+
	
				'<div class="label-20 disabled n123">123</div>'+
				
			'</div>'
		)
	})
});