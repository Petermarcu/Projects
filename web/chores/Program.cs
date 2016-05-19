using System.IO;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Builder;

namespace Chores
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var host = new WebHostBuilder()
                        .UseKestrel( options =>
                        {
                            //Configure SSL 
                            options.NoDelay = true;
                            options.UseHttps("cert.pfx", "testPassword");
                            options.UseConnectionLogging();
                        })
                        .UseUrls("http://localhost:5000", "https://localhost:5001")
                        .UseContentRoot(Directory.GetCurrentDirectory())
                        .UseStartup<Startup>()
                        .Build();

            host.Run();
        }
    }
}