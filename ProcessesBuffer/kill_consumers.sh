#!/bin/bash
# Script to terminate all consumer processes

# Get the process IDs of all running consumer programs
pids=$(ps aux | grep '[c]onsumer' | awk '{print $2}')

# Check if there are any PIDs found
if [ -z "$pids" ]; then
  echo "No consumer processes found."
else
  # Kill the processes
  echo "Terminating consumer processes with PIDs: $pids"
  kill -9 $pids
  echo "Consumer processes terminated."
fi
