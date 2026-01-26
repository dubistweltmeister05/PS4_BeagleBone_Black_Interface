#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/input/event1"
#define DEADZONE 10   // for 8-bit centered input

typedef struct {
    int lx, ly;
    int rx, ry;
    int l2, r2;
    int buttons[13];
} controller_t;

/* ---------- Helpers ---------- */

// Apply deadzone around center (128)
static int apply_deadzone(int value) {
    int centered = value - 128;

    if (centered > -DEADZONE && centered < DEADZONE)
        return 128;

    return value;
}

// Normalize 0–255 → -100 to +100
static int normalize_axis(int value) {
    int centered = value - 128;
    return (centered * 100) / 127;
}

// Normalize trigger 0–255 → 0–100
static int normalize_trigger(int value) {
    return (value * 100) / 255;
}

/* ---------- Event Processing ---------- */

void process_event(struct input_event *ev, controller_t *ctrl) {

    if (ev->type == EV_ABS) {

        int raw = ev->value;

        switch (ev->code) {

            case ABS_X:
                ctrl->lx = normalize_axis(apply_deadzone(raw));
                break;

            case ABS_Y:
                ctrl->ly = -normalize_axis(apply_deadzone(raw)); // invert Y
                break;

            case ABS_RX:
                ctrl->rx = normalize_axis(apply_deadzone(raw));
                break;

            case ABS_RY:
                ctrl->ry = -normalize_axis(apply_deadzone(raw)); // invert Y
                break;

            case ABS_Z:
                ctrl->l2 = normalize_trigger(raw);
                break;

            case ABS_RZ:
                ctrl->r2 = normalize_trigger(raw);
                break;
        }
    }

    if (ev->type == EV_KEY) {
        switch (ev->code) {
            case BTN_SOUTH: ctrl->buttons[0] = ev->value; break; // X
            case BTN_EAST:  ctrl->buttons[1] = ev->value; break; // O
            case BTN_NORTH: ctrl->buttons[2] = ev->value; break; // Triangle
            case BTN_WEST:  ctrl->buttons[3] = ev->value; break; // Square
            case BTN_TL:    ctrl->buttons[4] = ev->value; break;
            case BTN_TR:    ctrl->buttons[5] = ev->value; break;
            case BTN_TL2:   ctrl->buttons[6] = ev->value; break;
            case BTN_TR2:   ctrl->buttons[7] = ev->value; break;
            case BTN_SELECT:ctrl->buttons[8] = ev->value; break;
            case BTN_START: ctrl->buttons[9] = ev->value; break;
            case BTN_MODE:  ctrl->buttons[10] = ev->value; break;
            case BTN_THUMBL:ctrl->buttons[11] = ev->value; break;
            case BTN_THUMBR:ctrl->buttons[12] = ev->value; break;
        }
    }
}

/* ---------- Debug Print ---------- */

void print_controller(controller_t *c) {
    printf("LX:%4d LY:%4d | RX:%4d RY:%4d | L2:%3d R2:%3d | ",
           c->lx, c->ly, c->rx, c->ry, c->l2, c->r2);

    printf("Buttons: ");
    for (int i = 0; i < 13; i++) {
        printf("%d", c->buttons[i]);
    }
    printf("\n");
}

/* ---------- Main ---------- */

int main() {

    int fd = open(DEVICE, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open input device");
        return -1;
    }

    // Optional: exclusive access
    // ioctl(fd, EVIOCGRAB, 1);

    controller_t ctrl;
    memset(&ctrl, 0, sizeof(ctrl));

    struct input_event ev;

    while (1) {

        ssize_t n = read(fd, &ev, sizeof(ev));

        if (n == -1) {
            if (errno == EAGAIN) {
                usleep(1000);
                continue;
            } else {
                perror("read error");
                break;
            }
        }

        if (n != sizeof(ev)) {
            continue;
        }

        process_event(&ev, &ctrl);

        if (ev.type == EV_SYN) {
            print_controller(&ctrl);
        }
    }

    close(fd);
    return 0;
}
