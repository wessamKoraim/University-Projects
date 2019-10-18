PASSWORD="raspberrypi"
IP="10.100.28.246"
PIPATH="~/Amputude/test_measurements"
echo "Fetching all of the measurement files..."
sshpass -p $PASSWORD scp -r pi@$IP:$PIPATH .
echo "Fethed the files succesfully"

