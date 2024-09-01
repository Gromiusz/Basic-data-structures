#!/bin/bash

pids=$(ps aux | grep '[p]roducer' | awk '{print $2}')

if [ -z "$pids" ]; then
  echo "No producer processes found."
else
  echo "Terminating producer processes with PIDs: $pids"
  kill -9 $pids
  echo "producer processes terminated."
fi
