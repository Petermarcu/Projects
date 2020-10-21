using System;
using System.Drawing;
using System.IO;
using System.Collections.Generic;

namespace led_matrix
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string[]> imageList = new List<string[]>();

            for (int i = 1; i <= 9; i++)
            {                
                imageList.Add(transposeImage(String.Format("Link{0}", i)));
            }

            using (StreamWriter sw = new StreamWriter("output.txt"))
            {
                foreach (string[] image in imageList)
                {
                    foreach (string line in image)
                    {
                        sw.WriteLine(line);
                    }
                }
            }
        }

        public static string[] transposeImage(string filename)
        {
            Bitmap img = new Bitmap(String.Format(@"C:\Users\pmarcu\Desktop\Sprites\{0}.png", filename));
            /*for (int i = 0; i < img.Width; i++)
            {
                for (int j = 0; j < img.Height; j++)
                {
                    Color pixel = img.GetPixel(i,j);

                        Console.WriteLine("0x{0}{1}{2}, ", pixel.R.ToString("x2"), pixel.G.ToString("x2"), pixel.B.ToString("x2"));
                }
            } */

            int x = 0;
            int y = 31;
            int k = 2;
            Color pixel;
            string currentString = "";
            string[] transposedImage = new string[35];

            transposedImage[0] = String.Format("const long {0}[] PROGMEM = ", filename.ToUpper());
            transposedImage[1] = "{";


            for (y = 31; y >= 0; y--)
            {            
                for (x = 0; x < 8 ; x++)
                {
                    pixel = img.GetPixel(x,y);
                    currentString += String.Format("0x{0}{1}{2},", pixel.R.ToString("x2"), pixel.G.ToString("x2"), pixel.B.ToString("x2"));
                }

                y--;

                for (x = 7; x >= 0 ; x--)
                {
                    pixel = img.GetPixel(x,y);
                    currentString += String.Format("0x{0}{1}{2},", pixel.R.ToString("x2"), pixel.G.ToString("x2"), pixel.B.ToString("x2"));
                }
                
                transposedImage[k] = currentString;
                currentString = "";
                k++;

            }

            for (y = 0; y < 32; y++)
            {            
                for (x = 15; x >= 8 ; x--)
                {
                    pixel = img.GetPixel(x,y);
                    currentString += String.Format("0x{0}{1}{2},", pixel.R.ToString("x2"), pixel.G.ToString("x2"), pixel.B.ToString("x2"));
                }

                y++;

                for (x = 8; x < 16; x++)
                {
                    pixel = img.GetPixel(x,y);
                    currentString += String.Format("0x{0}{1}{2},", pixel.R.ToString("x2"), pixel.G.ToString("x2"), pixel.B.ToString("x2"));
                }
                
                transposedImage[k] = currentString;
                currentString = "";
                k++;
            }

            transposedImage[33] = transposedImage[31].Substring(0, transposedImage[31].Length - 1);

            transposedImage[34] = "};";
            return transposedImage;
        }

    }
}
