# PS4 BeagleBone Black Controller Interface

This project provides a robust interface for connecting and reading input from a PlayStation 4 (PS4) controller on a BeagleBone Black (BBB) running Linux. It includes:

- A C program to read and process controller input events
- Scripts for automatic Bluetooth connection
- A systemd service for seamless controller auto-connect at boot

---

## Features

- **Direct PS4 controller input reading** via Linux input events
- **Axis and trigger normalization** for easy integration with robotics or custom applications
- **Button state tracking**
- **Automatic Bluetooth connection** to a specified controller
- **Systemd integration** for auto-connect on boot

---

## Directory Structure

- `src/ps4_BBB_direct.c` — Main C source for reading and normalizing controller input
- `scripts/ps4-auto-connect.sh` — Bash script to auto-connect to a PS4 controller via Bluetooth
- `scripts/install.sh` — Installs the auto-connect script and systemd service
- `systemd/ps4-autoconnect.service` — Systemd service unit for auto-connecting the controller

---

## Setup & Installation

1. **Clone this repository** on your BeagleBone Black.

2. **Edit the Bluetooth MAC address** in `scripts/ps4-auto-connect.sh` to match your controller:
	```sh
	MAC="XX:XX:XX:XX:XX:XX"
	```

3. **Run the install script:**
	```sh
	sudo bash scripts/install.sh
	```
	This will:
	- Copy the auto-connect script to `/usr/local/bin/`
	- Copy and enable the systemd service
	- Reload systemd

4. **Reboot or start the service manually:**
	```sh
	sudo systemctl start ps4-autoconnect
	```

---

## Usage

### Reading Controller Input

Compile the C program:
```sh
gcc src/ps4_BBB_direct.c -o ps4_BBB_direct
```

Run the program (ensure your controller is connected and `/dev/input/event1` is correct):
```sh
sudo ./ps4_BBB_direct
```

The program will print normalized stick, trigger, and button values in real time.

### Auto-Connect on Boot

The systemd service will attempt to connect your controller automatically at boot using the MAC address you set.

---

## Troubleshooting

- **Controller not connecting?**
  - Double-check the MAC address in `ps4-auto-connect.sh`.
  - Ensure Bluetooth is enabled and working on the BBB.
  - Try running the script manually: `bash /usr/local/bin/ps4-auto-connect.sh`

- **Input device not found?**
  - The C program defaults to `/dev/input/event1`. Use `ls /dev/input/` to find the correct event device for your controller.
  - Edit the `#define DEVICE` line in the C source if needed.

---

## License

MIT License. See LICENSE file if present.

---

## Credits

- Inspired by open-source Linux gamepad and robotics projects.
