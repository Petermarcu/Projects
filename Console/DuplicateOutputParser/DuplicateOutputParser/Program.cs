using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text;

namespace DuplicateOutputParser
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string> listOfFilesToDelete = new List<string>();

            using (TextReader reader = File.OpenText(@"=c:\output.csv"))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    string[] splitLine = line.Split(',');
                    DetermineDeletes(splitLine, listOfFilesToDelete);
                }
            }

            foreach (string file in listOfFilesToDelete)
            {
                Console.WriteLine("del \"" + file + "\"");
            }
        }

        static void DetermineDeletes(string[] files, List<string> listOfFilesToDelete)
        {
            foreach (string file in files)
            {
                listOfFilesToDelete.Add(file);
            }
        }
    }
}
