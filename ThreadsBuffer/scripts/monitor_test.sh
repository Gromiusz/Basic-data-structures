echo "Testing monitors with low intensity (5%) of special messages"
sleep 4
./buffers_monitor_overwriting 19 1
echo "Test passed"
sleep 2

echo "Testing monitors with normal intensity (20%) of special messages"
sleep 3
./buffers_monitor_overwriting 4 1
echo "Test passed"
sleep 2

echo "Testing monitors with high intensity (50%) of special messages"
sleep 3
./buffers_monitor_overwriting 1 1
echo "Test passed"
sleep 2

echo "Testing monitors with high intensity (100%) of special messages"
sleep 3
./buffers_monitor_overwriting 0 1
echo "Test passed"
sleep 2