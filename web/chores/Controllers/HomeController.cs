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
            
            chores.Add(new Chore
            { 
                Id = 0,
                Name = "Clean room"
            });
            chores.Add(new Chore
            {
                Id = 1,
                Name = "Wash car"
            });
            chores.Add(new Chore
            {                
                Id = 2,
                Name = "Take out garbage"
            });
            
            return View(new HomeViewModel
            {
                ChoreList = chores
            });           
        }
    }
}