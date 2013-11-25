package com.ford.avarsdl.util;

import com.ford.syncV4.proxy.rpc.Address;
import com.ford.syncV4.proxy.rpc.Advertisement;
import com.ford.syncV4.proxy.rpc.EventDetails;
import com.ford.syncV4.proxy.rpc.Location;
import com.ford.syncV4.proxy.rpc.OnRadioDetails;
import com.ford.syncV4.proxy.rpc.RadioStation;
import com.ford.syncV4.proxy.rpc.SongInfo;
import com.ford.syncV4.proxy.rpc.Time;
import com.ford.syncV4.proxy.rpc.WebActivity;
import com.ford.syncV4.proxy.rpc.enums.TriggerSource;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 5:57 PM
 */
public class APIObjectsSimulator {

    public static OnRadioDetails getOnRadioDetails() {
        OnRadioDetails onRadioDetails = new OnRadioDetails();
        onRadioDetails.setTriggerSource(TriggerSource.TS_MENU);
        onRadioDetails.setLocation(getLocation());
        onRadioDetails.setAdvertisement(getAdvertisement());
        onRadioDetails.setEvent(getEventDetails());
        onRadioDetails.setRadioStation(getRadioStation());
        onRadioDetails.setSongInfo(getSongInfo());
        onRadioDetails.setWebActivity(getWebActivity());
        return onRadioDetails;
    }

    public static Location getLocation() {
        Location location = new Location();
        location.setGPSCoordinates("lat=46.463900&lon=30.738600");
        location.setAddress(getAddress());
        return location;
    }

    public static Address getAddress() {
        Address address = new Address();
        address.setState("State");
        address.setZipCode("65080");
        address.setStreet("Street");
        address.setCity("Odessa");
        return address;
    }

    public static Advertisement getAdvertisement() {
        Advertisement advertisement = new Advertisement();
        advertisement.setPhoneNumber("+3806700000");
        advertisement.setLocation(getLocation());
        advertisement.setCompanyName("Company Name");
        advertisement.setProductName("Product Name");
        return advertisement;
    }

    public static EventDetails getEventDetails() {
        EventDetails eventDetails = new EventDetails();
        eventDetails.setEventName("Event Name");
        eventDetails.setLocation(getLocation());
        eventDetails.setEventTime(getTime());
        eventDetails.setPhoneNumber("+380670000000");
        eventDetails.setPrice(65.0f);
        return eventDetails;
    }

    public static Time getTime() {
        Time time = new Time();
        time.setHours(20);
        time.setMinutes(20);
        time.setSeconds(20);
        time.setYear(2013);
        time.setMonth(11);
        time.setDay(12);
        time.setTZD("TZone");
        return time;
    }

    public static RadioStation getRadioStation() {
        RadioStation radioStation = new RadioStation();
        radioStation.setFrequency(102);
        radioStation.setFraction(5);
        radioStation.setAvailableHDs(2);
        return radioStation;
    }

    public static SongInfo getSongInfo() {
        SongInfo songInfo = new SongInfo();
        songInfo.setName("Name");
        songInfo.setArtist("Artists");
        songInfo.setGenre("30");
        songInfo.setAlbum("Album");
        songInfo.setYear(2013);
        songInfo.setDuration(180);
        return songInfo;
    }

    public static WebActivity getWebActivity() {
        WebActivity webActivity = new WebActivity();
        webActivity.setActionCode(123);
        webActivity.setURL("url");
        return webActivity;
    }
}