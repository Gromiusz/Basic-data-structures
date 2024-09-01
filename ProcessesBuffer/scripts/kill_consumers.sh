#!/bin/bash

pids=$(ps aux | grep '[c]onsumer' | awk '{print $2}')

if [ -z "$pids" ]; then
  echo "No consumer processes found."
else
  echo "Terminating consumer processes with PIDs: $pids"
  kill -9 $pids
  echo "Consumer processes terminated."
fi
