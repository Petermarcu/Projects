using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Chores.Models;

namespace Chores
{
    [Route("/api/[controller]")]
    public class ChoresController : Controller
    {
        private readonly ChoreContext _context;
        public ChoresController(ChoreContext context)
        {
            _context = context;
        }

        [HttpGet]
        public Task<List<Chore>> Get()
        {
            return _context.Chores.ToListAsync();
        }
        
        [HttpGet("{id:int}")]
        public Task<Chore> Get(int id)
        {
            return _context.Chores.SingleOrDefaultAsync(a => a.Id == id);
        }

        [HttpPost]
        public async Task<IActionResult> Post([FromBody]Chore chore)
        {
            if (!ModelState.IsValid)
            {
                return BadRequest(ModelState);
            }
            
            _context.Chores.Add(chore);
            await _context.SaveChangesAsync();
            return CreatedAtAction(nameof(Get), new { id = chore.Id }, chore);

        }
        
        [HttpDelete("{id:int}")]
        public Task Delete(int id)
        {
            var chore = new Chore
            {
                Id = id
            };
            _context.Attach(chore);
            _context.Remove(chore);
            return _context.SaveChangesAsync();
        }
    }
}

