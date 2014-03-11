package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.CompassDirection;
import com.ford.syncV4.proxy.rpc.enums.Dimension;
import com.ford.syncV4.proxy.rpc.enums.DisplayType;
import com.ford.syncV4.util.DebugTool;

public class GPSData extends RPCStruct {

    public GPSData() { }
    public GPSData(Hashtable hash) {
        super(hash);
    }
    public void setLongitudeDegrees(Integer longitudeDegrees) {
    	if (longitudeDegrees != null) {
    		store.put(Names.longitudeDegrees, longitudeDegrees);
    	} else {
    		store.remove(Names.longitudeDegrees);
    	}
    }
    public Integer getLongitudeDegrees() {
    	return (Integer) store.get(Names.longitudeDegrees);
    }
    public void setLatitudeDegrees(Integer latitudeDegrees) {
    	if (latitudeDegrees != null) {
    		store.put(Names.latitudeDegrees, latitudeDegrees);
    	} else {
    		store.remove(Names.latitudeDegrees);
    	}
    }
    public Integer getLatitudeDegrees() {
    	return (Integer) store.get(Names.latitudeDegrees);
    }
    public void setUtcYear(Integer utcYear) {
    	if (utcYear != null) {
    		store.put(Names.utcYear, utcYear);
    	} else {
    		store.remove(Names.utcYear);
    	}
    }
    public Integer getUtcYear() {
    	return (Integer) store.get(Names.utcYear);
    }
    public void setUtcMonth(Integer utcMonth) {
    	if (utcMonth != null) {
    		store.put(Names.utcMonth, utcMonth);
    	} else {
    		store.remove(Names.utcMonth);
    	}
    }
    public Integer getUtcMonth() {
    	return (Integer) store.get(Names.utcMonth);
    }
    public void setUtcDay(Integer utcDay) {
    	if (utcDay != null) {
    		store.put(Names.utcDay, utcDay);
    	} else {
    		store.remove(Names.utcDay);
    	}
    }
    public Integer getUtcDay() {
    	return (Integer) store.get(Names.utcDay);
    }
    public void setUtcHours(Integer utcHours) {
    	if (utcHours != null) {
    		store.put(Names.utcHours, utcHours);
    	} else {
    		store.remove(Names.utcHours);
    	}
    }
    public Integer getUtcHours() {
    	return (Integer) store.get(Names.utcHours);
    }
    public void setUtcMinutes(Integer utcMinutes) {
    	if (utcMinutes != null) {
    		store.put(Names.utcMinutes, utcMinutes);
    	} else {
    		store.remove(Names.utcMinutes);
    	}
    }
    public Integer getUtcMinutes() {
    	return (Integer) store.get(Names.utcMinutes);
    }
    public void setUtcSeconds(Integer utcSeconds) {
    	if (utcSeconds != null) {
    		store.put(Names.utcSeconds, utcSeconds);
    	} else {
    		store.remove(Names.utcSeconds);
    	}
    }
    public Integer getUtcSeconds() {
    	return (Integer) store.get(Names.utcSeconds);
    }
    public void setCompassDirection(CompassDirection compassDirection) {
    	if (compassDirection != null) {
    		store.put(Names.compassDirection, compassDirection);
    	} else {
    		store.remove(Names.compassDirection);
    	}
    }
    public CompassDirection getCompassDirection() {
        Object obj = store.get(Names.compassDirection);
        if (obj instanceof CompassDirection) {
            return (CompassDirection) obj;
        } else if (obj instanceof String) {
        	CompassDirection theCode = null;
            try {
                theCode = CompassDirection.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.compassDirection, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPdop(Integer pdop) {
    	if (pdop != null) {
    		store.put(Names.pdop, pdop);
    	} else {
    		store.remove(Names.pdop);
    	}
    }
    public Integer getPdop() {
    	return (Integer) store.get(Names.pdop);
    }
    public void setHdop(Integer hdop) {
    	if (hdop != null) {
    		store.put(Names.hdop, hdop);
    	} else {
    		store.remove(Names.hdop);
    	}
    }
    public Integer getHdop() {
    	return (Integer) store.get(Names.hdop);
    }
    public void setVdop(Integer vdop) {
    	if (vdop != null) {
    		store.put(Names.vdop, vdop);
    	} else {
    		store.remove(Names.vdop);
    	}
    }
    public Integer getVdop() {
    	return (Integer) store.get(Names.vdop);
    }
    public void setActual(Boolean actual) {
    	if (actual != null) {
    		store.put(Names.actual, actual);
    	} else {
    		store.remove(Names.actual);
    	}
    }
    public Boolean getActual() {
    	return (Boolean) store.get(Names.actual);
    }
    public void setSatellites(Integer satellites) {
    	if (satellites != null) {
    		store.put(Names.satellites, satellites);
    	} else {
    		store.remove(Names.satellites);
    	}
    }
    public Integer getSatellites() {
    	return (Integer) store.get(Names.satellites);
    }
    public void setDimension(Dimension dimension) {
    	if (dimension != null) {
    		store.put(Names.dimension, dimension);
    	} else {
    		store.remove(Names.dimension);
    	}
    }
    public Dimension getDimension() {
        Object obj = store.get(Names.dimension);
        if (obj instanceof Dimension) {
            return (Dimension) obj;
        } else if (obj instanceof String) {
        	Dimension theCode = null;
            try {
                theCode = Dimension.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.dimension, e);
            }
            return theCode;
        }
        return null;
    }
    public void setAltitude(Integer altitude) {
    	if (altitude != null) {
    		store.put(Names.altitude, altitude);
    	} else {
    		store.remove(Names.altitude);
    	}
    }
    public Integer getAltitude() {
    	return (Integer) store.get(Names.altitude);
    }
    public void setHeading(Integer heading) {
    	if (heading != null) {
    		store.put(Names.heading, heading);
    	} else {
    		store.remove(Names.heading);
    	}
    }
    public Integer getHeading() {
    	return (Integer) store.get(Names.heading);
    }
    public void setSpeed(Integer speed) {
    	if (speed != null) {
    		store.put(Names.speed, speed);
    	} else {
    		store.remove(Names.speed);
    	}
    }
    public Integer getSpeed() {
    	return (Integer) store.get(Names.speed);
    }
}
