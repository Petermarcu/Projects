using System;
using Bifrost.Devices.Gpio;
using Bifrost.Devices.Gpio.Core;
using System.Threading;
using Raspberry.IO.Components.Converters.Mcp3008;
using Raspberry.IO.GeneralPurpose;
using Raspberry.IO;

namespace IotSample
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Flashing Light!");

            var controller = GpioController.Instance;
            var pin = controller.OpenPin(5);
            pin.SetDriveMode(GpioPinDriveMode.Output);

            var pinValue = GpioPinValue.High;

            var bmp280 = new BMP280();
            bmp280.Initialize();

            const ConnectorPin adcClock = ConnectorPin.P1Pin23;
            const ConnectorPin adcMiso = ConnectorPin.P1Pin21;
            const ConnectorPin adcMosi = ConnectorPin.P1Pin19;
            const ConnectorPin adcCs = ConnectorPin.P1Pin24;

            var driver = new GpioConnectionDriver();

            var adcConnection = new Mcp3008SpiConnection(
                driver.Out(adcClock),
                driver.Out(adcCs),
                driver.In(adcMiso),
                driver.Out(adcMosi));

            while (true)
            {
                Thread.Sleep(500);

                // Read the temperature and pressure
                float p = bmp280.ReadPreasure().Result;
                float t = bmp280.ReadTemperature().Result;

                float tempF = (t * 9 / 5 + 32);
                float pressureInHg = p * .295357F / 1000;

                Console.WriteLine("Pressure: " + pressureInHg + " inHg");
                Console.WriteLine("Temperature: " + tempF + " F");

                // Bling the light
                if (pinValue == GpioPinValue.High)
                    pinValue = GpioPinValue.Low;
                else pinValue = GpioPinValue.High;

                pin.Write(pinValue);

                // Read the analog inputs
                AnalogValue a0 = adcConnection.Read(Mcp3008Channel.Channel0);
                AnalogValue a1 = adcConnection.Read(Mcp3008Channel.Channel1);
                AnalogValue a2 = adcConnection.Read(Mcp3008Channel.Channel2);

                Console.WriteLine("Value 1: " + a0.Value);
                Console.WriteLine("Value 2: " + a1.Value);
                Console.WriteLine("Value 3: " + a2.Value);
            }

        }
    }
}
