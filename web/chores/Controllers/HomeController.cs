using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Chores.Models;
using Chores.ViewModels;

namespace Chores.Controllers
{
    public class HomeController : Controller
    {
        [Route("")]
        public IActionResult Index()
        { 
            var chores = new List<Chore>();
            var availableChores = new List<Chore>();
            var myChores = new List<Chore>();
            var activeUser = "Peter";
            
            chores.Add(new Chore
            { 
                Id = 0,
                Name = "Clean room",
                Assignee = "Peter",
                Value = 200
            });
            chores.Add(new Chore
            {
                Id = 1,
                Name = "Wash car",
                Assignee = "Ashley",
                Value = 500
            });
            chores.Add(new Chore
            {                
                Id = 2,
                Name = "Take out garbage",
                Assignee = "Peter",
                Value = 200
            });
            chores.Add(new Chore
            {                
                Id = 3,
                Name = "Feed Fish",
                Assignee = "Unassigned",
                Value = 50
            });

            foreach (var chore in chores)
            {
                if (chore.Assignee == activeUser)
                {
                    myChores.Add(chore);
                }

                if (chore.Assignee == "Unassigned")
                {
                    availableChores.Add(chore);
                }
            }
            
            return View(new HomeViewModel
            {
                ChoreList = chores, AvailableChoreList = availableChores, MyChoreList = myChores, ActiveUser = activeUser
            });           
        }
    }
}