using Microsoft.EntityFrameworkCore;

namespace ServerApi.Models
{
    public class StorageContext : DbContext
    {
        public StorageContext(DbContextOptions<StorageContext> options)
            : base(options)
        {
        }

        public DbSet<Temperature> Temperatures { get; set; }
        public DbSet<Humidity> Humidities { get; set; }
    }
}