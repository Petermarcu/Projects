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
            
            chores.Add(new Chore("Clean room"));
            chores.Add(new Chore("Wash car"));
            chores.Add(new Chore("Take out garbage"));
            
            return View(new HomeViewModel
            {
                ChoreList = chores
            });           
        }
    }
}