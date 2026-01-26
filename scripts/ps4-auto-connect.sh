#!/bin/bash

MAC="92:A8:25:57:A4:AA"

# Wait for Bluetooth to fully initialize
 sleep 5

# Try multiple times (CSR dongle safety 😄)
 for i in {1..5}; do
     bluetoothctl connect $MAC && break
     sleep 2
done
