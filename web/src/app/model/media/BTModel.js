/**
 * @name MFT.BtModel
 *
 * @desc Bt Media data model
 *
 * @category	Model
 * @filesource	app/model/media/BtModel.js
 * @version		2.0
 *
 * @author		Gerashchenko Maksym
 */

MFT.BTModel = Em.Object.create({
    range:		MFT.RangedValue.create({value:-1, activeBand:0}),
    deviceList: {
        0: Em.Object.create({
            uid: 'BT1',
            name: MFT.locale.label.view_media_bt_device1,
            trackInfo: {title: 'Won\'t Get Fooled',artist:'The Who'}
        }),
        1: Em.Object.create({
            uid: 'BT2',
            name: MFT.locale.label.view_media_bt_device2,
            trackInfo: {title: 'Who Are You',artist:'The Who'}
        })
    }
});