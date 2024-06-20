#!/bin/bash

# Kill chip-lighting-app if it is running
chip_lighting_pid=$(pgrep -f chip-lighting-app)
if [ ! -z "$chip_lighting_pid" ]; then
    kill -9 "$chip_lighting_pid"
    echo "Killed chip-lighting-app with PID $chip_lighting_pid"
fi

# Remove /tmp/chip_* files and directories
sudo rm -rf /tmp/chip_*
echo "Removed /tmp/chip_* files and directories"

# Start chip-lighting-app
./chip-lighting-app
