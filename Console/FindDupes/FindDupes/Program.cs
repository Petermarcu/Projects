using System;
using System.Collections.Generic;
using System.IO;
using System.Security;
using System.Text;

class Program
{
    // Fields
    public static Dictionary<long, List<FileInfo>> fileSizeHashDictionary = new Dictionary<long, List<FileInfo>>();
    public static List<List<FileInfo>> duplicateFileList = new List<List<FileInfo>>();

    static void Main(string[] args)
    {
        if (args.Length < 1)
        {
            ShowUsage();
            return;
        }

        try
        {
            IEnumerable<FileInfo> listOfFiles = new DirectoryInfo(args[0]).EnumerateFiles("*", SearchOption.AllDirectories);
            List<List<FileInfo>> listOfIdenticalFiles = new List<List<FileInfo>>();

            foreach (FileInfo fileInfo in listOfFiles)
            {
                List<FileInfo> lengthList;
                if (!fileSizeHashDictionary.TryGetValue(fileInfo.Length, out lengthList))
                {
                    lengthList = new List<FileInfo>();
                    fileSizeHashDictionary.Add(fileInfo.Length, lengthList);
                }

                lengthList.Add(fileInfo);
            }

            FileInfo fileInfo1;
            FileInfo fileInfo2;

            foreach (long key in fileSizeHashDictionary.Keys)
            {
                // TODO: Go backwards instead and remove from the list as we find duplicates.
                // TODO: Prevent scanning the first in the list for each comparison. 
                List<FileInfo> sameLengthList = fileSizeHashDictionary[key];
                if (sameLengthList.Count > 1)
                {
                    while (sameLengthList.Count > 1)
                    {
                        List<FileInfo> currentIdenticalFilesList = new List<FileInfo>();

                        for (int j = 1; j < sameLengthList.Count; j++)
                        {
                            fileInfo1 = sameLengthList[0];
                            fileInfo2 = sameLengthList[j];

                            if ((bool)CompareFiles(fileInfo1, fileInfo2))
                            {
                                if (!currentIdenticalFilesList.Contains(fileInfo1))
                                {
                                    currentIdenticalFilesList.Add(fileInfo1);
                                }

                                if (!currentIdenticalFilesList.Contains(fileInfo2))
                                {
                                    currentIdenticalFilesList.Add(fileInfo2);
                                }
                            }
                        }

                        // Remove the first because its either tracked in the identical list or has no dupes.
                        sameLengthList.RemoveAt(0);

                        foreach (FileInfo fileInfo in currentIdenticalFilesList)
                        {
                            sameLengthList.Remove(fileInfo);
                        }

                        listOfIdenticalFiles.Add(currentIdenticalFilesList);
                        if (currentIdenticalFilesList.Count > 1)
                        {

                            StringBuilder sb = new StringBuilder();
                            foreach (FileInfo fi in currentIdenticalFilesList)
                            {
                                sb.Append(fi.FullName + ",");
                            }
                            //Console.WriteLine("Duplicate Count: " + currentIdenticalFilesList.Count.ToString());
                            Console.WriteLine(sb.ToString());
                        }
                    }
                }
            }
        }

        catch (ArgumentNullException ane)
        {
            Console.WriteLine("Path was null");
            Console.WriteLine(ane.Message + ane.Data);
            Console.WriteLine(ane.InnerException.Message + ane.Data);
        }
        catch (SecurityException se)
        {
            Console.WriteLine("Dont have access to the path");
            Console.WriteLine(se.Message + se.Data);
            Console.WriteLine(se.InnerException.Message + se.Data);
        }
        catch (PathTooLongException ptle)
        {
            Console.WriteLine("Path was too long");
            Console.WriteLine(ptle.Message + ptle.Data);
            Console.WriteLine(ptle.InnerException.Message + ptle.Data);
        }
        catch (ArgumentException ae)
        {
            Console.WriteLine("Invalid Path");
            Console.WriteLine(ae.Message + ae.Data);
            Console.WriteLine(ae.InnerException.Message + ae.Data);
        }

        // Console.ReadKey();
        return;
    }
    static void ShowUsage()
    {
        Console.WriteLine("");
        Console.WriteLine("Usage: LocateDuplicateFiles.exe <directory>");
        Console.WriteLine("");
        Console.WriteLine("Pass the directory to search recursively to the tool and it will output a csv in that directory with the directories name appended with .csv");
    }
    public static bool? CompareFiles(FileInfo targetFileInfo, FileInfo updatedFileInfo)
    {
        if (targetFileInfo.Length != updatedFileInfo.Length)
        {
            return false;
        }

        using (FileStream targetStream = File.OpenRead(targetFileInfo.FullName))
        {
            using (FileStream updatedStream = File.OpenRead(updatedFileInfo.FullName))
            {
                if (targetStream.Length != updatedStream.Length)
                {
                    return false;
                }

                // Using a larger buffer than the default buffer of 4 * 1024 used by FileStream.ReadByte improves performance.
                // The buffer size is based on user feedback. Based on performance results, a better buffer size may be determined.
                byte[] targetBuffer = new byte[16 * 1024];
                byte[] updatedBuffer = new byte[16 * 1024];
                int targetReadLength;
                int updatedReadLength;
                do
                {
                    targetReadLength = targetStream.Read(targetBuffer, 0, targetBuffer.Length);
                    updatedReadLength = updatedStream.Read(updatedBuffer, 0, updatedBuffer.Length);

                    if (targetReadLength != updatedReadLength)
                    {
                        return false;
                    }

                    for (int i = 0; i < targetReadLength; ++i)
                    {
                        if (targetBuffer[i] != updatedBuffer[i])
                        {
                            return false;
                        }
                    }

                } while (0 < targetReadLength);
            }
        }

        return true;
    }
}