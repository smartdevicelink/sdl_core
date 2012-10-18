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
		
		valueBinding:		'MFT.InfoController.appsScroll',
		
		pos:				0,
		
		coeficient:			0,
		
		scrollBarH:			0,

		buttonsScroll: Em.ContainerView.extend( Ember.TargetActionSupport, {
			classNames:	'buttonsScroll',
			elementId:	'buttonsScroll',
			actionUp:	function(){
				this._parentView.pos = this._parentView.value.y - this._parentView.value.startY;
				this._parentView.scrollBar.moveScrollBar();
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
					if( this._parentView._parentView.value.y < 0 ){
						this._parentView._parentView.pos += 52;
						this._parentView.moveScrollBar();
						this._parentView._parentView.value.scrollTo(0, this._parentView._parentView.value.y + 52, 200);
					}else if(this._parentView._parentView.value.y){
						this._parentView._parentView.pos = 0;
						this._parentView.moveScrollBar();
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
					if( this._parentView._parentView.value.scrollerH + this._parentView._parentView.value.y > this._parentView._parentView.value.wrapperH ){
						this._parentView._parentView.pos -= 52;
						this._parentView.moveScrollBar();
						this._parentView._parentView.value.scrollTo(0, this._parentView._parentView.value.y - 52, 200);
					}
				}
			}),
			
			moveScrollBar:	function(){
				this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.pos) * this._parentView.coeficient + 'px;' );
			}
		}),
		
		scrollObs:	function(){
			console.log(this.get('value.y'));
		}.observes('this.value.y'),
		
		loaded: function() {
			this.set('value', new iScroll('buttonsWrapper',{scrollbarClass: 'display:none'}));
			this.coeficient = this.value.wrapperH / this.value.scrollerH;
			this.scrollBarH = (this.value.wrapperH - 98) * this.coeficient;
			this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );
			this.scrollBar.moveScrollBar();
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