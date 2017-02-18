#!/usr/bin/env python

import paho.mqtt.publish as publish
import sys

topic = 	"lights/living_room"
msg_part = 	"door "
hostname =	"localhost"

if len(sys.argv) == 2:
	publish.single(topic, msg_part + sys.argv[1].lower(), hostname=hostname)
