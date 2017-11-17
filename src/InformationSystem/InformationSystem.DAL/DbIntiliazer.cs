using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;

namespace InformationSystem.DAL
{
    public class DbIntiliazer
    {
        public UserManager<IdentityUser> UserManager { get; }
        public RoleManager<IdentityRole> RoleManager { get; private set; }

        public DbIntiliazer(UserManager<IdentityUser> userManager, RoleManager<IdentityRole> roleManager)
        {
            UserManager = userManager;
            RoleManager = roleManager;
        }

        public async Task Initialize()
        {
            if (!RoleManager.Roles.Any(e => e.Name.Equals("Regular", StringComparison.InvariantCultureIgnoreCase)))
            {
                var identityRole = new IdentityRole("Regular");
                await RoleManager.CreateAsync(identityRole);
            }
            if (!RoleManager.Roles.Any(e => e.Name.Equals("Mechanic", StringComparison.InvariantCultureIgnoreCase)))
            {
                var identityRole = new IdentityRole("Mechanic");
                await RoleManager.CreateAsync(identityRole);
            }
            if (!RoleManager.Roles.Any(e => e.Name.Equals("Admin", StringComparison.InvariantCultureIgnoreCase)))
            {
                var identityRole = new IdentityRole("admin");
                await RoleManager.CreateAsync(identityRole);
            }
            if ((await UserManager.FindByNameAsync("admin@iis.mrnustik.net")) == null)
            {
                var identityUser = new IdentityUser
                {
                    Email = "admin@iis.mrnustik.net",
                    UserName = "admin@iis.mrnustik.net"
                };
                await UserManager.CreateAsync(identityUser, "admin");
                await UserManager.AddToRoleAsync(identityUser, "Admin");
            }
        }
    }
}
