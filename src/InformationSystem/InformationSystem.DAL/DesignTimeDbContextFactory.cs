using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace InformationSystem.DAL
{
    class DesignTimeDbContextFactory :IDesignTimeDbContextFactory<ServiceDbContext>
    {
        public ServiceDbContext CreateDbContext(string[] args)
        {
            var builder = new DbContextOptionsBuilder<ServiceDbContext>();
            builder.UseSqlServer("Server=tcp:university-db.database.windows.net,1433;Initial Catalog=IISDatabase;Persist Security Info=False;User ID=Mrnda;Password=Mrnustik112233;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;",
                optionsBuilder => optionsBuilder.MigrationsAssembly(typeof(ServiceDbContext).GetTypeInfo().Assembly.GetName().Name));
            return new ServiceDbContext(builder.Options);
        }
    }
}
