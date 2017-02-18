#!/usr/bin/env python

import paho.mqtt.publish as publish
import sys

topic = 	"lights/kitchen"
msg_part = 	"all "
hostname =	"localhost"

if len(sys.argv) == 2:
	publish.single(topic, msg_part + sys.argv[1].lower(), hostname=hostname)
