using System;

namespace SoilMoistureSensorCalibratedPump.Tests.Integration
{
    public class GrowSenseIrrigatorHardwareTestHelper : GrowSenseHardwareTestHelper
    {
        public int SimulatorPumpPin = 2;

        public GrowSenseIrrigatorHardwareTestHelper ()
        {
        }

        public override void PrepareDeviceForTest (bool consoleWriteDeviceOutput)
        {
            base.PrepareDeviceForTest (false);

            SetDevicePumpOffTime (0);

            if (consoleWriteDeviceOutput)
                ReadFromDeviceAndOutputToConsole ();
        }

        public void SetDevicePumpOffTime (int pumpOffTime)
        {
            var cmd = "O" + pumpOffTime;

            Console.WriteLine ("");
            Console.WriteLine ("Setting pump off time to " + pumpOffTime + " seconds...");
            Console.WriteLine ("  Sending '" + cmd + "' command to device");
            Console.WriteLine ("");

            SendDeviceCommand (cmd);
        }
    }
}
