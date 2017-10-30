using System;
using System.Collections.Generic;
using System.Text;
using InformationSystem.DAL.Entities;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;

namespace InformationSystem.DAL
{
    class ServiceDbContext : IdentityDbContext<UserEntity, RoleEntity, string>
    {
        public ServiceDbContext(DbContextOptions<ServiceDbContext> options) : base(options)
        {
        }

        public DbSet<CarEntity> Cars { get; set; }
        public DbSet<RepairEntity> Repairs { get; set; }
        public DbSet<MaterialEntity> Materials { get; set; }
        
    }
}
