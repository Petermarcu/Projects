namespace Chores.Models
{
    public class Chore
    {
        public string Name { get; set; }

        public Chore(string name)
        {
            this.Name = name;
        }
    }
}
