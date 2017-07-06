using System;
using Bifrost.Devices.Gpio;
using Bifrost.Devices.Gpio.Core;
using System.Threading;

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

            while (true)
            {
                pin.Write(pinValue);

                float p = bmp280.ReadPreasure().Result;
                float t = bmp280.ReadTemperature().Result;

                float tempF = (t * 9 / 5 + 32);
                float pressureInHg = p * .295357F / 1000;

                Console.WriteLine("Pressure: " + pressureInHg + " inHg");
                Console.WriteLine("Temperature: " + tempF + " F");

                Thread.Sleep(500);

                if (pinValue == GpioPinValue.High)
                    pinValue = GpioPinValue.Low;
                else pinValue = GpioPinValue.High;
            }

        }
    }
}
