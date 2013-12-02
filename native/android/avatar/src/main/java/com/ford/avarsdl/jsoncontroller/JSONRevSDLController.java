package com.ford.avarsdl.jsoncontroller;

import com.ford.avarsdl.requests.CancelAccessCommand;
import com.ford.avarsdl.requests.GrantAccessCommand;
import com.ford.avarsdl.requests.RequestCommand;
import com.ford.avarsdl.requests.StartScanCommand;
import com.ford.avarsdl.requests.StopScanCommand;
import com.ford.avarsdl.requests.TuneDownCommand;
import com.ford.avarsdl.requests.TuneRadioCommand;
import com.ford.avarsdl.requests.TuneUpCommand;
import com.ford.avarsdl.util.Logger;
import com.ford.avarsdl.util.RPCConst;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

public class JSONRevSDLController extends JSONController {

    private final Hashtable<String, RequestCommand> commandsHashTable;

    public JSONRevSDLController() {
        super(RPCConst.CN_REVSDL);

        commandsHashTable = new Hashtable<String, RequestCommand>();
        initializeCommandsTable();
    }

    @Override
    protected void processResponse(String response) {
        Logger.i(getClass().getSimpleName() + " Response: " + response);
        processRegistrationResponse(response);
    }

    @Override
    protected void processRequest(String request) {
        String method = mJSONParser.getMethod();
        method = method.substring(method.indexOf('.') + 1, method.length());
        Logger.d(getClass().getSimpleName() + " request: " + request);
        RequestCommand requestCommand = commandsHashTable.get(method);
        if (requestCommand != null) {
            requestCommand.execute(mJSONParser.getId(), mJSONParser.getParams());
        } else {
            Logger.w(getClass().getSimpleName() + " unknown request");
        }
    }

    private void initializeCommandsTable() {
        GrantAccessCommand grantAccessCommand = new GrantAccessCommand();
        commandsHashTable.put(Names.GrantAccess, grantAccessCommand);

        CancelAccessCommand cancelAccessCommand = new CancelAccessCommand();
        commandsHashTable.put(Names.CancelAccess, cancelAccessCommand);

        StartScanCommand startScanCommand = new StartScanCommand();
        commandsHashTable.put(Names.StartScan, startScanCommand);

        StopScanCommand stopScanCommand = new StopScanCommand();
        commandsHashTable.put(Names.StopScan, stopScanCommand);

        TuneRadioCommand tuneRadioCommand = new TuneRadioCommand();
        commandsHashTable.put(Names.TuneRadio, tuneRadioCommand);

        TuneUpCommand tuneUpCommand = new TuneUpCommand();
        commandsHashTable.put(Names.TuneUp, tuneUpCommand);

        TuneDownCommand tuneDownCommand = new TuneDownCommand();
        commandsHashTable.put(Names.TuneDown, tuneDownCommand);
    }
}