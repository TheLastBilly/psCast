#!/bin/bash

APP_NAME=pscast
FTP_IP=192.168.1.222
FTP_PORT=1337

UNCOMPRESS_DIR=vitaupload
mkdir ${UNCOMPRESS_DIR}

unzip ${APP_NAME}.vpk -d ${UNCOMPRESS_DIR}

ftp -n ${FTP_IP} ${FTP_PORT} <<END_SCRIPT
put vitaupload/eboot.bin ux0:/app/PCAST0001/eboot.bin
END_SCRIPT
rm -rf $UNCOMPRESS_DIR