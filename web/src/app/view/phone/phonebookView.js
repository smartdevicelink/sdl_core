/**
 * @name MFT.PhonebookPhoneView
 *
 * @desc Phone Phonebook module visual representation
 *
 * @category	View
 * @filesource	app/view/phone/PhonebookView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.PhonebookPhoneView = Em.ContainerView.create( MFT.LoadableView, {

    elementId:	'phone_phonebook',

    childViews: [
        'help',
        'content'
    ],

    help: Em.View.extend({
        elementId:		'phone_phonebook_help',

        classNames:			['blured_background','shifted']
    }),
    // INIT PHONE BOOK
    afterRender: function(){
        this._super();
        FLAGS.ANDROID_PHONE_BOOK ? FFW.PhoneBookRPC.getContacts('AF') : FFW.PhoneBookRPC.getContacts_em('AF');

    },

    content: Em.ContainerView.extend({

        elementId:	'phone_phonebook_content',

        childViews: [
            'filters',
            'list'
        ],

        filters: MFT.ButtonGroup.extend({

            elementId:			'phone_phonebook_filters',

            activeIndex: 0,

            action: 'onPhoneBookFilterChange',

            target: 'MFT.PhoneController',

            items: [
                {
                    params:{
                        text:'A-F',
                        range:'AF'
                    }
                },
                {
                    params:{
                        text:'G-L',
                        range:'GL'
                    }
                },
                {
                    params:{
                        text:'M-S',
                        range:'MS'
                    }
                },
                {
                    params:{
                        text:'T-Z',
                        range:'TZ'
                    }
                },
                {
                    params:{
                        text:'Other',
                        range:'other'
                    }
                }
            ]
        }),

        list: MFT.List.extend({
            elementId:		   'phone_phonebook_list',

            /** Items */
            items: [],

            itemsOnPage:	5
        })
    })
});