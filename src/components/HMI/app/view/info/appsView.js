/**
 * @name MFT.InfoAppsview
 * 
 * @desc Info Apps visual representation
 * 
 * @category	View
 * @filesource	app/view/info/AppsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.InfoAppsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'info_apps',
	
	classNameBindings: ['MFT.States.info.apps.active:active_state','MFT.helpMode'],
	
	classNames: ['hidden'],
	
	stateObj: MFT.States.info.apps,

	childViews: [
		'vehicleHealthReport',
		'Asist911',
		'installButton',
		'findNewApps', 
		'buttonsWrapper'
	],
	
	vehicleHealthReport:   MFT.Button.extend({
		goToState:			'vehicle.healthReport',
		classNames:			'button vehicleHealthReport',
		icon:				'images/info/ico_vehicle.png',
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
		elementId:			'infoAppsVehicleHealthReport',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	Asist911:   MFT.Button.extend({
		goToState:			'help.helpAssist',
		classNames:			'button Asist911',
		icon:				'images/info/ico_assist.png',
		textBinding:		'MFT.locale.label.view_info_apps_911Assist',
		elementId:			'infoAppsAsist911',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	installButton:   MFT.Button.extend({
		goToState:  		'settings.system.installApplications',
		icon:				'images/info/ico_info_install.png',
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_InstallApplicationsUp',
		elementId:			'infoAppsInstallButton',
		classNames:			'button installButton',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	findNewApps:   MFT.Button.extend({
		goToState:			'settings.system.installApplications',
		/*icon:				'images/info/ico_info_install.png',*/
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_FindNewApplications',
		elementId:			'infoAppsFindNewApps',
		classNames:			'button findNewApps',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),

	buttonsWrapper: Em.ContainerView.extend({
		classNames:			'buttonsWrapper',
		elementId:			'buttonsWrapper',
		childViews: [
			'buttonsScroll',
			'scrollBar'
		],
		
		scroll:				null,

		pos:				0,

		scrollPos: 			0,

		coeficient:			0,
		
		scrollBarH:			0,

		buttonsScroll: Em.ContainerView.extend( Ember.TargetActionSupport, {
			classNames:	'buttonsScroll',
			elementId:	'buttonsScroll',
			actionUp:	function(){
				this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
			}
		}),

		scrollBar: Em.ContainerView.extend({
			classNames:			'scrollBar',
			elementId:			'scrollBar',
			childViews: [
				'scrollArrowUp',
				'scroller',
				'scrollArrowDown'
			],

			scrollArrowUp:   MFT.Button.extend({
				classNames:	'scrollArrows scrollArrowUp button',
				click:		function(){
					MFT.InfoAppsView.buttonsWrapper.scroll.scrollTo(0, MFT.InfoAppsView.buttonsWrapper.scroll.y + 52, 200);
					if( MFT.InfoAppsView.buttonsWrapper.scroll.y < -52){
						MFT.InfoAppsView.buttonsWrapper.set('scrollPos', MFT.InfoAppsView.buttonsWrapper.scroll.y + 52);
					}else{
						MFT.InfoAppsView.buttonsWrapper.set('scrollPos', 0);
					}
				}
			}),

			scroller: Em.View.extend({
				classNames:	'scroller',
				attributeBindings: ['style'],
				style: 'margin-top: 0px'
			}),

			scrollArrowDown:   MFT.Button.extend({
				classNames:	'scrollArrows scrollArrowDown button',
				click:		function(){
					MFT.InfoAppsView.buttonsWrapper.scroll.scrollTo(0, MFT.InfoAppsView.buttonsWrapper.scroll.y - 52, 200);
					if( Math.abs(MFT.InfoAppsView.buttonsWrapper.scroll.y) < ((MFT.InfoAppsView.buttonsWrapper.scroll.scrollerH - MFT.InfoAppsView.buttonsWrapper.scroll.wrapperH) - 52) ){
						MFT.InfoAppsView.buttonsWrapper.set('scrollPos', MFT.InfoAppsView.buttonsWrapper.scroll.y - 52);
					}else{
						MFT.InfoAppsView.buttonsWrapper.set('scrollPos',  (MFT.InfoAppsView.buttonsWrapper.scroll.scrollerH - MFT.InfoAppsView.buttonsWrapper.scroll.wrapperH));
					}
				}
			}),
			
			moveScrollBar:	function(){
				
				this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
			}.observes('this.parentView.scrollPos'),
		}),
		
		scrollEnd:	function(){
			if( MFT.InfoAppsView.buttonsWrapper.scroll.y > 0){
				this.set('scrollPos', 0);
			}else if( Math.abs(MFT.InfoAppsView.buttonsWrapper.scroll.y) > (MFT.InfoAppsView.buttonsWrapper.scroll.scrollerH - MFT.InfoAppsView.buttonsWrapper.scroll.wrapperH) ){
				this.set('scrollPos', MFT.InfoAppsView.buttonsWrapper.scroll.scrollerH - MFT.InfoAppsView.buttonsWrapper.scroll.wrapperH);
			}else{
				this.set('scrollPos', MFT.InfoAppsView.buttonsWrapper.scroll.y);
			}
		},
		
		loaded: function() {
			this.set('scroll', new iScroll('buttonsWrapper',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.InfoAppsView.buttonsWrapper.scrollEnd();}}));
			this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
			this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
			this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );
		}
	}),

	afterRender: function() {

		var butt;

		for(var i in MFT.InfoController.get('appsParams')){

			butt = MFT.Button.create({
				goToState:			MFT.InfoController.get('appsParams')[i].goToState,
				classNames:			MFT.InfoController.get('appsParams')[i].classNames,
				icon:				MFT.InfoController.get('appsParams')[i].icon,
				textBinding:		MFT.InfoController.get('appsParams')[i].textBinding,
				arrow:				MFT.InfoController.get('appsParams')[i].arrow,
				action:				MFT.InfoController.get('appsParams')[i].action,
				target:				MFT.InfoController.get('appsParams')[i].target,
				disabledBinding:	MFT.InfoController.get('appsParams')[i].disabledBinding,
				onDown:				MFT.InfoController.get('appsParams')[i].onDown
			});

			MFT.InfoAppsView.buttonsWrapper.buttonsScroll.get('_childViews').pushObject(butt);
		}

		
		setTimeout(function () { MFT.InfoAppsView.buttonsWrapper.loaded(); }, 200);
		
	}
});