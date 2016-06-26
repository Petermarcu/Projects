using System.ComponentModel.DataAnnotations;

namespace Chores.Models
{
    public class Chore
    {
        public int Id {get; set; }
        
        [Required]
        public string Name { get; set; }

        public string Assignee {get; set; }

        public int Value {get; set;}
        
    }
}
