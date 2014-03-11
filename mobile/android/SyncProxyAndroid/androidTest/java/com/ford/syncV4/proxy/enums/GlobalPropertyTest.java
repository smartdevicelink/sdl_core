package com.ford.syncV4.proxy.enums;

import android.test.AndroidTestCase;

import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;

/**
 * Created by Andrew Batutin on 11/18/13.
 */
public class GlobalPropertyTest extends AndroidTestCase {

    public void testGlobalPropertiesEnumHasHelppromtValue() throws Exception {
        assertNotNull("", GlobalProperty.HELPPROMPT);
    }

    public void testGlobalPropertiesEnumHasTimeoutpromtValue() throws Exception {
        assertNotNull("", GlobalProperty.TIMEOUTPROMPT);
    }

    public void testGlobalPropertiesEnumHasVrhelptitleValue() throws Exception {
        assertNotNull("", GlobalProperty.VRHELPTITLE);
    }

    public void testGlobalPropertiesEnumHasVrhelpitemsValue() throws Exception {
        assertNotNull("", GlobalProperty.VRHELPITEMS);
    }

    public void testGlobalPropertiesEnumHasMenunameValue() throws Exception {
        assertNotNull("", GlobalProperty.MENUNAME);
    }

    public void testGlobalPropertiesEnumHasMenuiconValue() throws Exception {
        assertNotNull("", GlobalProperty.MENUICON);
    }

    public void testGlobalPropertiesEnumHasKeyboardpropertiesValue() throws Exception {
        assertNotNull("", GlobalProperty.KEYBOARDPROPERTIES);
    }
}
