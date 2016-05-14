using System.Collections.Generic;
using Chores.Models;

namespace Chores.ViewModels
{
    public class HomeViewModel
    {
        public IEnumerable<Chore> ChoreList { get; set; }
    }
}

