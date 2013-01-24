/**
 * @name MFT.AudioPassThruPopUp
 * 
 * @desc AudioPassThruPopUp module visual representation
 * 
 * @category    View
 * @filesource    app/view/applink/AudioPassThruPopUp.js
 * @version        2.0
 *
 * @author        Andriy Melnik
 */

MFT.AudioPassThruPopUp = Em.ContainerView.create({

    elementId:              'AudioPassThruPopUp',

    classNames:             'AudioPassThruPopUp',

    classNameBindings:      ['activate:AudioPassThruActive'],

    childViews: [
        'applicationName',
        'image',
        'message1',
        'message2',
        'message3',
        'buttonRetry',
        'buttonDone',
        'buttonCancel'
    ],

    content1:           'Title',

    content2:           'Text',

    activateBinding:    'MFT.ApplinkModel.AudioPassThruState',

    timer:              null,

    applicationName :    MFT.Label.extend({

        elementId:              'AudioPassThruPopUpApplicationName',

        classNames:             'applicationName',

        contentBinding:         'parentView.appName'
    }),

    /**
     * Wagning image on Alert PopUp
     */
    image:  Em.View.extend({
        elementId:              'audioPassThruImage',

        classNames:             'audioPassThruImage',
    }),

    message1 : MFT.Label.extend({

        elementId:              'AudioPassThruPopUpMessage1',

        classNames:             'message1',

        contentBinding:         'parentView.content1'
    }),

    message2 : MFT.Label.extend({

        elementId:              'AudioPassThruPopUpMessage2',

        classNames:             'message2',

        contentBinding:         'parentView.content2'
    }),

    message3 : MFT.Label.extend({

        elementId:              'AudioPassThruPopUpMessage3',

        classNames:             'message3',

        contentBinding:         'parentView.content3'
    }),

    buttonRetry: MFT.Button.create({
        elementId:      'AudioPassThruPopUpButtonRetry',
        classNames:     'buttonRetry softButton',
        text:           'Retry',
        responseResult: 'RETRY',
        actionUp: function(){
            MFT.ApplinkController.callPerformAudioPassThruPopUpResponse( this );
        }
    }),

    buttonDone: MFT.Button.create({
        elementId:      'AudioPassThruPopUpButtonDone',
        classNames:     'buttonDone softButton',
        text:           'Done',
        responseResult: 'SUCCESS',
        actionUp: function(){
            MFT.ApplinkController.callPerformAudioPassThruPopUpResponse( this );
        }
    }),

    buttonCancel: MFT.Button.create({
        elementId:      'AudioPassThruPopUpButtonCancel',
        classNames:     'buttonCancel softButton',
        text:           'Cancel',
        responseResult: 'ABORTED',
        actionUp: function(){
            MFT.ApplinkController.callPerformAudioPassThruPopUpResponse( this );
        }
    }),

    /**
     * Method clears PopUp's timer when activity flag become false,
     * and show PopUp with data come from ApplinkCorel when activity flag become true
     */
    AudioPassThruActivity: function(){
        
        if( this.activate ){
            
            var self = this,
                data = MFT.ApplinkModel.AudioPassThruData;

            MFT.ApplinkModel.onPrompt(data.initialPrompt.ttsChunks);

            this.set('appName',    MFT.ApplinkController.getApplicationModel(data.appId).appName);

            this.set('content1',    data.audioPassThruDisplayText1);
            this.set('content2',    data.audioPassThruDisplayText2);
            
            clearTimeout(this.timer);
            this.timer = setTimeout(function(){
                MFT.ApplinkController.performAudioPassThruResponse("SUCCESS");
            }, data.maxDuration);
        }else{
            if( this.timer ){
                clearTimeout( this.timer );
            }
        }
    }.observes('activate'),
});