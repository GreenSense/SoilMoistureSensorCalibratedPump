GARDEN_HOSTNAME=garden
GARDEN_USER=j

# Get environmental variables
IRRIGATOR_PORT=$IRRIGATOR_PORT
SIMULATOR_PORT=$IRRIGATOR_SIMULATOR_PORT

if [ ! "IRRIGATOR_PORT" ]; then
  IRRIGATOR_PORT="/dev/ttyUSB0"
fi
if [ ! "SIMULATOR_PORT" ]; then
  SIMULATOR_PORT="/dev/ttyUSB1"
fi

