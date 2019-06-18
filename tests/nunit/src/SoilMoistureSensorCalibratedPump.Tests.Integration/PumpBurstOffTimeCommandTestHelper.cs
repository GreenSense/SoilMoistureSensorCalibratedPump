﻿using System;

namespace SoilMoistureSensorCalibratedPump.Tests.Integration
{
    public class PumpBurstOffTimeCommandTestHelper : SerialCommandTestHelper
    {
        public int PumpBurstOffTime = 1;

        public void TestPumpBurstOffTimeCommand ()
        {
            Letter = "O";
            Value = PumpBurstOffTime;
            Label = "pump burst off time";

            TestCommand ();
        }
    }
}