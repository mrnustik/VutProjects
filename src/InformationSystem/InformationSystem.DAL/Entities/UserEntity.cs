using System.Text;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.DAL.Entities
{
    public class UserEntity : IdentityUser
    {
        public string Name { get; set; }
    }
}
