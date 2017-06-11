using System;

namespace CodeAdventure.Maze
{
    public class Maze
    {
        public Maze(int width = 40, int height = 30)
        {
            Height = height;
            Width = width;
            _maze = new char[height][];
            for (int i = 0; i < height; i++)
            {
                _maze[i] = new char[width];
            }
        }

        public void Fill(char fillValue)
        {
            for (int h = 0; h < Height; h++)
            {
                for (int w = 0; w < Width; w++)
                {
                    _maze[h][w] = fillValue;
                }
            }
        }

        public void SetBorder(char borderValue)
        {
            for (int w = 0; w < Width; w++)
            {
                _maze[0][w] = borderValue;
                _maze[Height - 1][w] = borderValue;
            }

            for (int h = 1; h < Height - 1; h++)
            {
                _maze[h][0] = borderValue;
                _maze[h][Width - 1] = borderValue;
            }
        }

        public void SetStart(char startValue = 'S',int x = 0, int y = 1)
        {
            _maze[y][x] = startValue; 
        }

        public void SetEnd(char endValue, int x, int y)
        {
            _maze[y][x] = endValue; 
        }

        public char GetValue(int x, int y)
        {
            return _maze[y][x];
        }
        
        public int Height { get; }
        public int Width { get; }

        private char[][] _maze;
    }
}
