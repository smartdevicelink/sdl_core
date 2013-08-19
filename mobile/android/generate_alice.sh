#!/bin/bash

rsync -avc --inplace --delete-excluded --progress --exclude='out/' --exclude='gen/' --exclude='bin/' SyncProxyTester/ SPTAlice
perl -i -p -e 's/com\.ford\.syncV4\.android/com.ford.syncV4.SPTAlice/g' $(ack -lQ 'com.ford.syncV4.android' SPTAlice)
perl -i -p -e 's/SyncProxyTester/SPTAlice/g' SPTAlice/res/values/strings.xml SPTAlice/src/com/ford/syncV4/android/constants/Const.java
mv SPTAlice/src/com/ford/syncV4/{android,SPTAlice}

