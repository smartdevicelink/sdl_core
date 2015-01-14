#!/bin/bash
wget http://19.7.95.92/uploads/note/attachment/12/libFordMediaCore.so
chmod 775 libFordMediaCore.so
mv libFordMediaCore.so plugins/com/ford/mediacore

wget http://19.7.95.92/uploads/note/attachment/11/libFordPhoneCore.so
chmod 775 libFordPhoneCore.so
mv libFordPhoneCore.so plugins/com/ford/phonecore
