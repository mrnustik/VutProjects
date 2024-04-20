using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.DAL.Entities
{
    public class ApplicationUser :IdentityUser
    {
        public string Name { get; set; }
        public bool IsEnabled { get; set; } = true;

    }
}
