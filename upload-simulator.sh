# May need be run as sudo

# Specify a temporary directory name
SIMULATOR_TMP_DIR="_simulatortmp"

# Remove old versions
rm -rf $SIMULATOR_TMP_DIR

# Make a new directory
mkdir -p $SIMULATOR_TMP_DIR
cd $SIMULATOR_TMP_DIR

# Clone the latest version
git clone https://github.com/CompulsiveCoder/ArduinoSerialController.git && \

cd ArduinoSerialController && \

# Upload
sh upload-to-port.sh "/dev/ttyACM0" && \

# Remove the temporary directory
rm -rf $SIMULATOR_TMP_DIR
