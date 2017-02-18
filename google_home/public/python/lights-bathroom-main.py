#!/usr/bin/env python

import paho.mqtt.publish as publish
import sys

topic = 	"lights/bathroom"
msg_part = 	"main "
hostname =	"localhost"

if len(sys.argv) == 2:
	publish.single(topic, msg_part + sys.argv[1].lower(), hostname=hostname)
