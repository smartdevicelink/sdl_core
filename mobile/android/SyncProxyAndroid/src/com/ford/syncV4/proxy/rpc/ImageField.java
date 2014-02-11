package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.ImageFieldName;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class ImageField extends RPCStruct {
    public ImageField() {
    }

    public ImageField(Hashtable hash) {
        super(hash);
    }

    public ImageFieldName getName() {
        Object obj = store.get(Names.name);
        if (obj instanceof ImageFieldName) {
            return (ImageFieldName) obj;
        } else if (obj instanceof String) {
            ImageFieldName theCode = null;
            try {
                theCode = ImageFieldName.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.name, e);
            }
            return theCode;
        }
        return null;
    }

    public void setName(ImageFieldName imageFieldName) {
        if (imageFieldName != null) {
            store.put(Names.name, imageFieldName);
        } else {
            store.remove(Names.name);
        }
    }

    public Vector<FileType> getImageTypeSupported() {
        if (store.get(Names.imageTypeSupported) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) store.get(Names.imageTypeSupported);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof FileType) {
                    return (Vector<FileType>) list;
                }
            }
        }
        return null;
    }

    public void setImageTypeSupported(Vector<FileType> imageFieldName) {
        if (imageFieldName != null) {
            store.put(Names.imageTypeSupported, imageFieldName);
        } else {
            store.remove(Names.imageTypeSupported);
        }
    }

    public ImageResolution getImageResolution() {
        Object obj = store.get(Names.imageResolution);
        if (obj instanceof ImageResolution) {
            return (ImageResolution) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ImageResolution((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.imageResolution, e);
            }
        }
        return null;
    }

    public void setImageResolution(ImageResolution imageResolution) {
        if (imageResolution != null) {
            store.put(Names.imageResolution, imageResolution);
        } else {
            store.remove(Names.imageResolution);
        }
    }
}