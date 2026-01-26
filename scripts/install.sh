#!/bin/bash

sudo cp scripts/ps4-auto-connect.sh /usr/local/bin/
sudo chmod +x /usr/local/bin/ps4-auto-connect.sh

sudo cp systemd/ps4-autoconnect.service /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable ps4-autoconnect
