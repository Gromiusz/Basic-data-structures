#!/bin/bash
# Script to terminate all consumer processes

# Get the process IDs of all running consumer programs
pids=$(ps aux | grep '[p]roducer' | awk '{print $2}')

# Check if there are any PIDs found
if [ -z "$pids" ]; then
  echo "No producer processes found."
else
  # Kill the processes
  echo "Terminating producer processes with PIDs: $pids"
  kill -9 $pids
  echo "producer processes terminated."
fi
