using System.Collections.Generic;
using Chores.Models;

namespace Chores.ViewModels
{
    public class HomeViewModel
    {
        public IEnumerable<Chore> ChoreList { get; set; }
        public IEnumerable<Chore> MyChoreList { get; set; }
        public IEnumerable<Chore> AvailableChoreList { get; set; }
        public string ActiveUser {get; set; }
    }
}

