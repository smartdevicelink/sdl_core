package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class ShowConstantTBT extends RPCRequest {

    public ShowConstantTBT() {
        super("ShowConstantTBT");
    }
    public ShowConstantTBT(Hashtable hash) {
        super(hash);
    }
    public void setNavigationText1(String navigationText1) {
        if (navigationText1 != null) {
            parameters.put(Names.navigationText1, navigationText1);
        } else {
        	parameters.remove(Names.navigationText1);
        }
    }
    public String getNavigationText1() {
        return (String) parameters.get(Names.navigationText1);
    }
    public void setNavigationText2(String navigationText2) {
        if (navigationText2 != null) {
            parameters.put(Names.navigationText2, navigationText2);
        } else {
        	parameters.remove(Names.navigationText2);
        }
    }
    public String getNavigationText2() {
        return (String) parameters.get(Names.navigationText2);
    }
    public void setEta(String eta) {
        if (eta != null) {
            parameters.put(Names.eta, eta);
        } else {
        	parameters.remove(Names.eta);
        }
    }
    public String getEta() {
        return (String) parameters.get(Names.eta);
    }
    public void setTotalDistance(String totalDistance) {
        if (totalDistance != null) {
            parameters.put(Names.totalDistance, totalDistance);
        } else {
        	parameters.remove(Names.totalDistance);
        }
    }
    public String getTotalDistance() {
        return (String) parameters.get(Names.totalDistance);
    }
    public void setTurnIcon(Image turnIcon) {
        if (turnIcon != null) {
            parameters.put(Names.turnIcon, turnIcon);
        } else {
        	parameters.remove(Names.turnIcon);
        }
    }
    public Image getTurnIcon() {
        Object obj = parameters.get(Names.turnIcon);
        if (obj instanceof Image) {
            return (Image) obj;
        } else {
        	return new Image((Hashtable) obj);
        }
    }
    public void setDistanceToManeuver(Float distanceToManeuver) {
        if (distanceToManeuver != null) {
            parameters.put(Names.distanceToManeuver, distanceToManeuver);
        } else {
        	parameters.remove(Names.distanceToManeuver);
        }
    }
    public Float getDistanceToManeuver() {
        return (Float) parameters.get(Names.distanceToManeuver);
    }
    public void setDistanceToManeuverScale(Float distanceToManeuverScale) {
        if (distanceToManeuverScale != null) {
            parameters.put(Names.distanceToManeuverScale, distanceToManeuverScale);
        } else {
        	parameters.remove(Names.distanceToManeuverScale);
        }
    }
    public Float getDistanceToManeuverScale() {
        return (Float) parameters.get(Names.distanceToManeuverScale);
    }
    public void setManeuverComplete(Boolean maneuverComplete) {
        if (maneuverComplete != null) {
            parameters.put(Names.maneuverComplete, maneuverComplete);
        } else {
        	parameters.remove(Names.maneuverComplete);
        }
    }
    public Boolean getManeuverComplete() {
        return (Boolean) parameters.get(Names.maneuverComplete);
    }
    public void setSoftButtons(Vector<SoftButton> softButtons) {
        if (softButtons != null) {
            parameters.put(Names.softButtons, softButtons);
        } else {
        	parameters.remove(Names.softButtons);
        }
    }
    public Vector<SoftButton> getSoftButtons() {
        if (parameters.get(Names.softButtons) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.softButtons);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SoftButton) {
	                return (Vector<SoftButton>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<SoftButton> newList = new Vector<SoftButton>();
	                for (Object hashObj : list) {
	                    newList.add(new SoftButton((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
}
