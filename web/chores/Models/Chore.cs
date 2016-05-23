using System.ComponentModel.DataAnnotations;

namespace Chores.Models
{
    public class Chore
    {
        public int Id {get; set; }
        
        [Required]
        public string Name { get; set; }
    }
}
