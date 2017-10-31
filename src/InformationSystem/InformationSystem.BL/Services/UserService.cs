using System;
using System.Collections.Generic;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.DAL;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.BL.Services
{
    public enum UserType
    {
        Regular,
        Mechanic,
        Admin
    }

    public class UserService
    {
        public UserManager<IdentityUser> UserManager { get; }
        public SignInManager<IdentityUser> SignInManager { get; }

        public UserService(UserManager<IdentityUser> userManager, SignInManager<IdentityUser> signInManager)
        {
            UserManager = userManager;
            SignInManager = signInManager;
        }
        
        public async Task<IdentityResult> CreateUserAsync(string email, string password, UserType userType = UserType.Regular)
        {
            var identityUser = new IdentityUser()
            {
                UserName = email,
                Email = email
            };
            var result = await UserManager.CreateAsync(identityUser, password);
            if (result.Succeeded)
            {
                return await UserManager.AddToRoleAsync(identityUser, UserTypeToRoleName(userType));
            }
            return result;
        }

        public async Task<bool> SignInAsync(string email, string password)
        {
            
            var user = await UserManager.FindByEmailAsync(email);
            if (user != null)
            {
                if (await SignInManager.CanSignInAsync(user))
                {
                    await SignInManager.SignInAsync(user, false);
                    return true;
                }
            }
            return false;
        }

        public Task SignOutAsync()
        {
            return SignInManager.SignOutAsync();
        }
        
        private string UserTypeToRoleName(UserType userType)
        {
            switch (userType)
            {
                case UserType.Regular:
                    return "Regular";
                case UserType.Mechanic:
                    return "Mechanic";
                case UserType.Admin:
                    return "Admin";
            }
            return String.Empty;
        }
    }
}
