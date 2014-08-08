package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;
import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 07.08.14
 * Time: 14:33
 */
public class SendLocation extends RPCRequest {

    public SendLocation() {
        super(Names.SendLocation);
    }

    public SendLocation(Hashtable hash) {
        super(hash);
    }

    public SendLocation(RPCRequest that) {
        super(that);
    }

    public void setLongitudeDegrees(Double longitudeDegrees) {
        if (longitudeDegrees != null) {
            parameters.put(Names.longitudeDegrees, longitudeDegrees);
        } else {
            parameters.remove(Names.longitudeDegrees);
        }
    }

    public Double getLongitudeDegrees() {
        if (!parameters.containsKey(Names.longitudeDegrees)) {
            return null;
        }
        if (parameters.get(Names.longitudeDegrees) instanceof Double) {
            return (Double) parameters.get(Names.longitudeDegrees);
        }
        return null;
    }

    public void setLatitudeDegrees(Double latitudeDegrees) {
        if (latitudeDegrees != null) {
            parameters.put(Names.latitudeDegrees, latitudeDegrees);
        } else {
            parameters.remove(Names.latitudeDegrees);
        }
    }

    public Double getLatitudeDegrees() {
        if (!parameters.containsKey(Names.latitudeDegrees)) {
            return null;
        }
        if (parameters.get(Names.latitudeDegrees) instanceof Double) {
            return (Double) parameters.get(Names.latitudeDegrees);
        }
        return null;
    }

    /**
     * Set Name / Title of intended location
     *
     * @param locationName Name / Title of intended location
     */
    public void setLocationName(String locationName) {
        if (locationName != null) {
            parameters.put(Names.locationName, locationName);
        } else {
            parameters.remove(Names.locationName);
        }
    }

    /**
     * @return Name / Title of intended location
     */
    public String getLocationName() {
        if (!parameters.containsKey(Names.locationName)) {
            return null;
        }
        if (parameters.get(Names.locationName) instanceof String) {
            return (String) parameters.get(Names.locationName);
        }
        return null;
    }

    /**
     * Set Description intended location / establishment (if applicable)
     *
     * @param locationName Description intended location / establishment (if applicable)
     */
    public void setLocationDescription(String locationName) {
        if (locationName != null) {
            parameters.put(Names.locationDescription, locationName);
        } else {
            parameters.remove(Names.locationDescription);
        }
    }

    /**
     * @return Description intended location / establishment (if applicable)
     */
    public String getLocationDescription() {
        if (!parameters.containsKey(Names.locationDescription)) {
            return null;
        }
        if (parameters.get(Names.locationDescription) instanceof String) {
            return (String) parameters.get(Names.locationDescription);
        }
        return null;
    }

    /**
     * Location address (if applicable)
     *
     * @param addressLines Location address (if applicable)
     */
    public void setAddressLines(Vector<String> addressLines) {
        if (addressLines != null) {
            parameters.put(Names.addressLines, addressLines);
        } else {
            parameters.remove(Names.addressLines);
        }
    }

    /**
     * @return Location address (if applicable)
     */
    public Vector<String> getAddressLines() {
        if (parameters.get(Names.addressLines) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>)parameters.get(Names.addressLines);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof String) {
                    return (Vector<String>)list;
                }
            }
        }
        return null;
    }

    /**
     * Set Phone number of intended location / establishment (if applicable)
     *
     * @param phoneNumber Phone number of intended location / establishment (if applicable)
     */
    public void setPhoneNumber(String phoneNumber) {
        if (phoneNumber != null) {
            parameters.put(Names.phoneNumber, phoneNumber);
        } else {
            parameters.remove(Names.phoneNumber);
        }
    }

    /**
     * @return Phone number of intended location / establishment (if applicable)
     */
    public String getPhoneNumber() {
        if (!parameters.containsKey(Names.phoneNumber)) {
            return null;
        }
        if (parameters.get(Names.phoneNumber) instanceof String) {
            return (String) parameters.get(Names.phoneNumber);
        }
        return null;
    }

    /**
     * Set Image / icon of intended location (if applicable and supported)
     *
     * @param locationImage Image / icon of intended location (if applicable and supported)
     */
    public void setLocationImage(Image locationImage) {
        if (locationImage != null) {
            parameters.put(Names.locationImage, locationImage);
        } else {
            parameters.remove(Names.locationImage);
        }
    }

    /**
     * @return Image / icon of intended location (if applicable and supported)
     */
    public Image getLocationImage() {
        if (!parameters.containsKey(Names.locationImage)) {
            return null;
        }
        if (parameters.get(Names.locationImage) instanceof Image) {
            return (Image) parameters.get(Names.locationImage);
        }
        return null;
    }
}