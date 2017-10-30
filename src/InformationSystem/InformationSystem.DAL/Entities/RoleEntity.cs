using Microsoft.AspNetCore.Identity;

namespace InformationSystem.DAL.Entities
{
    public class RoleEntity : IdentityRole
    {
        public string Description { get; set; }
    }
}