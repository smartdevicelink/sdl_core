/*
 * simplectrl.c
 * This file is part of OsciPrime
 *
 * Copyright (C) 2011 - Manuel Di Cerbo
 *
 * OsciPrime is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * OsciPrime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OsciPrime; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// Source: http://android.serverbox.ch/?p=262


#include <stdio.h>
#include <libusb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define IN 0x81
#define OUT 0x02

#define VID_NEXUSS 0x18D1
#define PID_NEXUSS 0x4E22

#define VID_NEXUS7 0x18d1
#define PID_NEXUS7 0x4e42

#define VID_MOTOROLA_RAZR 0x22b8
#define PID_MOTOROLA_RAZR 0x4362

#define VID_HUAWEI 0x12d1
#define PID_HUAWEI 0x1052

#define VID VID_HUAWEI
#define PID PID_HUAWEI

#define ACCESSORY_VID 0x18d1
#define ACCESSORY_PID 0x2D01
#define ACCESSORY_PID_ALT 0x2D00

#define LEN 2

#define ERR_DEVICE_NOT_OPEN -2

/*
 If you are on Ubuntu you will require libusb as well as the headers...
 We installed the headers with "apt-get source libusb"
 gcc simplectrl.c -I/usr/include/ -o simplectrl -lusb-1.0 -I/usr/include/ -I/usr/include/libusb-1.0
 
 Tested for Nexus S with Gingerbread 2.3.4
 */

typedef unsigned char byte;

static int mainPhase();
static int init(void);
static int deInit(void);
static void error(int code);
static void status(int code);
static int setupAccessory(
                          const char* manufacturer,
                          const char* modelName,
                          const char* description,
                          const char* version,
                          const char* uri,
                          const char* serialNumber);
static int connectToAccessory();

//static
static struct libusb_device_handle* handle;
//static char stop;
//static char success = 0;

int main (int argc, char *argv[]){
	int ret = init();
    switch (ret) {
        case ERR_DEVICE_NOT_OPEN: {
            if (connectToAccessory() < 0) {
                fprintf(stdout, "Error connecting to accessory\n");
                deInit();
                return -1;
            }
            break;
        }
            
        case 0: {
            //doTransfer();
            if(setupAccessory(
                              "Nexus-Computing GmbH",
                              "osciprime",
                              "Description",
                              "antilope",
                              "http://android.serverbox.ch/?p=262",
                              "2254711SerialNo.") < 0){
                fprintf(stdout, "Error setting up accessory\n");
                deInit();
                return -1;
            };
            break;
        }
            
        default:
            // other error
            return -1;
    }
	if(mainPhase() < 0){
		fprintf(stdout, "Error during main phase\n");
		deInit();
		return -1;
	}
	deInit();
	fprintf(stdout, "Done, no errors\n");
	return 0;
}

static void printCharArray(unsigned char *buf, unsigned int len) {
    // http://stackoverflow.com/questions/5189071/print-part-of-char-array/5191818#5191818
    fprintf(stdout, "%.*s\n", len, buf);
}

static void intToByteArray(int i, byte **outArray, int *outLen) {
    *outLen = 4;
    byte *buf = (byte *)malloc(*outLen);
    buf[0] = (i >> 24) & 0xFF;
    buf[1] = (i >> 16) & 0xFF;
    buf[2] = (i >> 8) & 0xFF;
    buf[3] = i & 0xFF;
    *outArray = buf;
}

static int mainPhase(){
	unsigned char buffer[500000];
	int response = 0;
	int transferred;
    
    // reading data length
	response = libusb_bulk_transfer(handle,IN,buffer,16384, &transferred,0);
    fprintf(stdout, "1: Transferred %d bytes\n", transferred);
	if(response < 0){error(response);return -1;}
    if (transferred > 1) {
        printCharArray(buffer, transferred);
    } else {
        fprintf(stdout, "Length: %u\n", buffer[0]);
    }
    
    // reading data
	response = libusb_bulk_transfer(handle,IN,buffer,sizeof(buffer), &transferred,0);
    fprintf(stdout, "2: Transferred %d bytes\n", transferred);
	if(response < 0){error(response);return -1;}
    printCharArray(buffer, transferred);
    int dataLength = transferred;
    
    // writing data length
    byte *lenBuf = 0;
    int lenBufLen = 0;
    intToByteArray(transferred, &lenBuf, &lenBufLen);
    printCharArray(lenBuf, lenBufLen);
    
    response = libusb_bulk_transfer(handle, OUT, lenBuf, lenBufLen, &transferred, 0);
    free(lenBuf), lenBuf = 0;
    if (response < 0) {
        error(response);
        return -1;
    }
    printf("Sent %d bytes\n", transferred);
    
    // writing data
    response = libusb_bulk_transfer(handle, OUT, buffer, dataLength, &transferred, 0);
    if (response < 0) {
        error(response);
        return -1;
    }
    printf("Sent %d bytes\n", transferred);
    
    return 0;
}


static int init(){
	int response = libusb_init(NULL);
    if (response != 0) {
        error(response);
        return -1;
    }
	if((handle = libusb_open_device_with_vid_pid(NULL, VID, PID)) == NULL){
		fprintf(stdout, "Problem acquiring handle for device\n");
		return ERR_DEVICE_NOT_OPEN;
	}
	libusb_claim_interface(handle, 0);
	return 0;
}

static int connectToAccessory() {
    fprintf(stdout, "Waiting for accessory\n");
    int tries = 10;
	for(;;){//attempt to connect to new PID, if that doesn't work try ACCESSORY_PID_ALT
        fprintf(stdout, "%d ", tries);
	fflush(stdout);
		tries--;
		if((handle = libusb_open_device_with_vid_pid(NULL, ACCESSORY_VID, ACCESSORY_PID)) == NULL){
			if(tries < 0){
                fprintf(stdout, "\nProblem acquiring handle for accessory\n");
				return -1;
			}
		}else{
			break;
		}
		sleep(1);
	}
	libusb_claim_interface(handle, 0);
	fprintf(stdout, "\nInterface claimed, ready to transfer data\n");
    return 0;
}

static int deInit(){
	//TODO free all transfers individually...
	//if(ctrlTransfer != NULL)
	//	libusb_free_transfer(ctrlTransfer);
	if(handle != NULL)
		libusb_release_interface (handle, 0);
	libusb_exit(NULL);
	return 0;
}

static int setupAccessory(
                          const char* manufacturer,
                          const char* modelName,
                          const char* description,
                          const char* version,
                          const char* uri,
                          const char* serialNumber){
    
	unsigned char ioBuffer[2];
	int devVersion;
	int response;
    
	response = libusb_control_transfer(
                                       handle, //handle
                                       0xC0, //bmRequestType
                                       51, //bRequest
                                       0, //wValue
                                       0, //wIndex
                                       ioBuffer, //data
                                       2, //wLength
                                       0 //timeout
                                       );
    
	if(response < 0){error(response);return-1;}
    
	devVersion = ioBuffer[1] << 8 | ioBuffer[0];
	fprintf(stdout,"Verion Code Device: %d\n", devVersion);
	
	usleep(1000);//sometimes hangs on the next transfer :(
    
	response = libusb_control_transfer(handle,0x40,52,0,0,(char*)manufacturer,strlen(manufacturer),0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,1,(char*)modelName,strlen(modelName)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,2,(char*)description,strlen(description)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,3,(char*)version,strlen(version)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,4,(char*)uri,strlen(uri)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,5,(char*)serialNumber,strlen(serialNumber)+1,0);
	if(response < 0){error(response);return -1;}
    
	fprintf(stdout,"Accessory Identification %d sent\n", devVersion);
    
	response = libusb_control_transfer(handle,0x40,53,0,0,NULL,0,0);
	if(response < 0){error(response);return -1;}
    
	fprintf(stdout,"Attempted to put device into accessory mode %d\n", devVersion);
    
	if(handle != NULL) {
		libusb_release_interface (handle, 0);
        libusb_close(handle);
    }
    
    return connectToAccessory();
}

static void error(int code){
	fprintf(stdout,"\n");
	switch(code){
        case LIBUSB_ERROR_IO:
            fprintf(stdout,"Error: LIBUSB_ERROR_IO\nInput/output error.\n");
            break;
        case LIBUSB_ERROR_INVALID_PARAM:
            fprintf(stdout,"Error: LIBUSB_ERROR_INVALID_PARAM\nInvalid parameter.\n");
            break;
        case LIBUSB_ERROR_ACCESS:
            fprintf(stdout,"Error: LIBUSB_ERROR_ACCESS\nAccess denied (insufficient permissions).\n");
            break;
        case LIBUSB_ERROR_NO_DEVICE:
            fprintf(stdout,"Error: LIBUSB_ERROR_NO_DEVICE\nNo such device (it may have been disconnected).\n");
            break;
        case LIBUSB_ERROR_NOT_FOUND:
            fprintf(stdout,"Error: LIBUSB_ERROR_NOT_FOUND\nEntity not found.\n");
            break;
        case LIBUSB_ERROR_BUSY:
            fprintf(stdout,"Error: LIBUSB_ERROR_BUSY\nResource busy.\n");
            break;
        case LIBUSB_ERROR_TIMEOUT:
            fprintf(stdout,"Error: LIBUSB_ERROR_TIMEOUT\nOperation timed out.\n");
            break;
        case LIBUSB_ERROR_OVERFLOW:
            fprintf(stdout,"Error: LIBUSB_ERROR_OVERFLOW\nOverflow.\n");
            break;
        case LIBUSB_ERROR_PIPE:
            fprintf(stdout,"Error: LIBUSB_ERROR_PIPE\nPipe error.\n");
            break;
        case LIBUSB_ERROR_INTERRUPTED:
            fprintf(stdout,"Error:LIBUSB_ERROR_INTERRUPTED\nSystem call interrupted (perhaps due to signal).\n");
            break;
        case LIBUSB_ERROR_NO_MEM:
            fprintf(stdout,"Error: LIBUSB_ERROR_NO_MEM\nInsufficient memory.\n");
            break;
        case LIBUSB_ERROR_NOT_SUPPORTED:
            fprintf(stdout,"Error: LIBUSB_ERROR_NOT_SUPPORTED\nOperation not supported or unimplemented on this platform.\n");
            break;
        case LIBUSB_ERROR_OTHER:
            fprintf(stdout,"Error: LIBUSB_ERROR_OTHER\nOther error.\n");
            break;
        default:
            fprintf(stdout, "Error: unkown error\n");
	}
}

static void status(int code){
	fprintf(stdout,"\n");
	switch(code){
		case LIBUSB_TRANSFER_COMPLETED:
			fprintf(stdout,"Success: LIBUSB_TRANSFER_COMPLETED\nTransfer completed.\n");
			break;
		case LIBUSB_TRANSFER_ERROR:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_ERROR\nTransfer failed.\n");
			break;
		case LIBUSB_TRANSFER_TIMED_OUT:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_TIMED_OUT\nTransfer timed out.\n");
			break;
		case LIBUSB_TRANSFER_CANCELLED:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_CANCELLED\nTransfer was cancelled.\n");
			break;
		case LIBUSB_TRANSFER_STALL:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_STALL\nFor bulk/interrupt endpoints: halt condition detected (endpoint stalled).\nFor control endpoints: control request not supported.\n");
			break;
		case LIBUSB_TRANSFER_NO_DEVICE:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_NO_DEVICE\nDevice was disconnected.\n");
			break;
		case LIBUSB_TRANSFER_OVERFLOW:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_OVERFLOW\nDevice sent more data than requested.\n");
			break;
		default:
			fprintf(stdout,"Error: unknown error\nTry again(?)\n");
			break;
	}
}
