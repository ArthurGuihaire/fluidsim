#!/bin/bash

# Check if the system is Arch Linux
if ! grep -qi "ID=arch" /etc/os-release; then
    echo "Not Arch Linux. Exiting."
    exit 0
fi

# Get the username of the person who invoked sudo (or fallback to current user)
REAL_USER="${SUDO_USER:-$USER}"

# Find the user's session
USER_SESSION=$(loginctl | grep "$REAL_USER" | awk '{print $1}' | head -n 1)

# Get the session type (Wayland or X11)
SESSION_TYPE=$(loginctl show-session "$USER_SESSION" -p Type | cut -d= -f2)

# Debug output (optional)
echo "Detected session type: $SESSION_TYPE"

# Run the appropriate script
if [ "$SESSION_TYPE" = "wayland" ]; then
    echo "Detected Wayland session. Running Wayland installer..."
    chmod +x ./installation_arch_wayland.sh
    ./installation_arch_wayland.sh
elif [ "$SESSION_TYPE" = "x11" ]; then
    echo "Detected X11 session. Running X11 installer..."
    chmod +x ./installation_arch_x11.sh
    ./installation_arch_x11.sh
else
    echo "Unknown session type or not in a graphical session. Exiting."
    exit 0
fi
