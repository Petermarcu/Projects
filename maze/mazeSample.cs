using System;
using CodeAdventure.Maze;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Here is your Maze!");
        Maze maze = new Maze();
        maze.Fill(' ');
        maze.SetBorder('#');
        maze.SetStart();
        maze.SetEnd('E', maze.Width - 1, maze.Height - 2);
        for (int y = 0; y < maze.Height; y++)
        {
            string row = "";
            for (int x = 0; x < maze.Width; x++)
            {
                //TODO: use string builder
                row += maze.GetValue(x, y);
            }   
            Console.WriteLine(row);
        }
        
    }
}
