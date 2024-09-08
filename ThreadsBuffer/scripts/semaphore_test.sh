echo "Testing semaphores with low intensity (5%) of special messages"
sleep 4
./buffers_semaphore 19 1
echo "Test passed"
sleep 2

echo "Testing semaphores with normal intensity (20%) of special messages"
sleep 3
./buffers_semaphore 4 1
echo "Test passed"
sleep 2

echo "Testing semaphores with high intensity (50%) of special messages"
sleep 3
./buffers_semaphore 1 1
echo "Test passed"
sleep 2

echo "Testing semaphores with high intensity (100%) of special messages"
sleep 3
./buffers_semaphore 0 1
echo "Test passed"
sleep 2