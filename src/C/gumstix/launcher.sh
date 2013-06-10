#!/bin/sh

server_ip=10.42.0.66
port_listen=5000
port_send=5500
device_mkusb=/dev/ttyUSB0

./reseau $server_ip $port_listen $port_send $device_mkusb
