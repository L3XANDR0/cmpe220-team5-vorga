#!/bin/bash

echo -n "14" > /sys/class/gpio/export
echo -n "15" > /sys/class/gpio/export
echo -n "49" > /sys/class/gpio/export

echo out > /sys/class/gpio/gpio14/direction
echo out > /sys/class/gpio/gpio15/direction
echo out > /sys/class/gpio/gpio49/direction
