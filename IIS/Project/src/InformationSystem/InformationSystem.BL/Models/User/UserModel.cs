using System;
using System.Collections.Generic;
using System.Text;

namespace InformationSystem.BL.Models.User
{
    public class UserModel
    {
        public string Name { get; set; }
        public string Email { get; set; }
        public string Role { get; set; }
        public bool IsEnabled { get; set; }
        
    }
}
