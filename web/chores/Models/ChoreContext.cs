using Microsoft.EntityFrameworkCore;

namespace Chores.Models
{
    public class ChoreContext : DbContext 
    { 
        public ChoreContext(DbContextOptions<ChoreContext> options) : base(options)  
        { 
            Database.EnsureCreated(); 
        } 
        public DbSet<Chore> Chores { get; set; } 
    } 
} 
