using System.Runtime.InteropServices;
using Microsoft.AspNetCore.Mvc;

namespace Chores
{
    [Route("/api/[controller]")]
    public class PlatformController : Controller
    {
        [HttpGet]
        public string Get()
        {
            return RuntimeInformation.OSDescription;
        }
    }
}
