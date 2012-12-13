package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TirePressureTellTale;
import com.ford.syncV4.util.DebugTool;

public class TireStatus extends RPCStruct {

    public TireStatus() { }
    public TireStatus(Hashtable hash) {
        super(hash);
    }
    public void setPressureTellTale(TirePressureTellTale pressureTellTale) {
    	if (pressureTellTale != null) {
    		store.put(Names.pressureTellTale, pressureTellTale);
    	} else {
    		store.remove(Names.pressureTellTale);
    	}
    }
    public TirePressureTellTale getPressureTellTale() {
        Object obj = store.get(Names.pressureTellTale);
        if (obj instanceof TirePressureTellTale) {
            return (TirePressureTellTale) obj;
        } else if (obj instanceof String) {
        	TirePressureTellTale theCode = null;
            try {
                theCode = TirePressureTellTale.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.pressureTellTale, e);
            }
            return theCode;
        }
        return null;
    }
    public void setLeftFront(SingleTireStatus leftFront) {
    	if (leftFront != null) {
    		store.put(Names.leftFront, leftFront);
    	} else {
    		store.remove(Names.leftFront);
    	}
    }
    public SingleTireStatus getLeftFront() {
    	Object obj = store.get(Names.leftFront);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.leftFront, e);
            }
        }
        return null;
    }
    public void setRightFront(SingleTireStatus rightFront) {
    	if (rightFront != null) {
    		store.put(Names.rightFront, rightFront);
    	} else {
    		store.remove(Names.rightFront);
    	}
    }
    public SingleTireStatus getRightFront() {
    	Object obj = store.get(Names.rightFront);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rightFront, e);
            }
        }
        return null;
    }
    public void setLeftRear(SingleTireStatus leftRear) {
    	if (leftRear != null) {
    		store.put(Names.leftRear, leftRear);
    	} else {
    		store.remove(Names.leftRear);
    	}
    }
    public SingleTireStatus getLeftRear() {
    	Object obj = store.get(Names.leftRear);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.leftRear, e);
            }
        }
        return null;
    }
    public void setRightRear(SingleTireStatus rightRear) {
    	if (rightRear != null) {
    		store.put(Names.rightRear, rightRear);
    	} else {
    		store.remove(Names.rightRear);
    	}
    }
    public SingleTireStatus getRightRear() {
    	Object obj = store.get(Names.rightRear);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rightRear, e);
            }
        }
        return null;
    }
    public void setInnerLeftRear(SingleTireStatus innerLeftRear) {
    	if (innerLeftRear != null) {
    		store.put(Names.innerLeftRear, innerLeftRear);
    	} else {
    		store.remove(Names.innerLeftRear);
    	}
    }
    public SingleTireStatus getInnerLeftRear() {
    	Object obj = store.get(Names.innerLeftRear);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.innerLeftRear, e);
            }
        }
        return null;
    }
    public void setInnerRightRear(SingleTireStatus innerRightRear) {
    	if (innerRightRear != null) {
    		store.put(Names.innerRightRear, innerRightRear);
    	} else {
    		store.remove(Names.innerRightRear);
    	}
    }
    public SingleTireStatus getInnerRightRear() {
    	Object obj = store.get(Names.innerRightRear);
        if (obj instanceof SingleTireStatus) {
            return (SingleTireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new SingleTireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.innerRightRear, e);
            }
        }
        return null;
    }
}
