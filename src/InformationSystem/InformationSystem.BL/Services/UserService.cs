using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;


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
        public UserManager<ApplicationUser> UserManager { get; }
        public SignInManager<ApplicationUser> SignInManager { get; }

        public UserService(UserManager<ApplicationUser> userManager, SignInManager<ApplicationUser> signInManager)
        {
            UserManager = userManager;
            SignInManager = signInManager;
        }

        public async Task<IQueryable<UserModel>> GetAllUsers()
        {
            var admins = (await UserManager.GetUsersInRoleAsync("admin")).Select(u => new UserModel { Name= u.Name,  Email = u.Email, Role = "admin"});
            var mechanics = (await UserManager.GetUsersInRoleAsync("Mechanic")).Select(u => new UserModel {Name = u.Name, Email = u.Email, Role = "Mechanic" });
            var users = (await UserManager.GetUsersInRoleAsync("Regular")).Select(u => new UserModel { Name = u.Name, Email = u.Email, Role = "Regular" });
            return admins.Concat(mechanics).Concat(users).AsQueryable();
        }
        
        public async Task<IdentityResult> CreateUserAsync(string name, string email, string password, UserType userType = UserType.Regular)
        {
            var identityUser = new ApplicationUser()
            {
                Name = name,
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
                if (await UserManager.CheckPasswordAsync(user, password))
                {
                    if (await SignInManager.CanSignInAsync(user))
                    {
                        await SignInManager.SignInAsync(user, false);
                        return true;
                    }
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

        public async Task DeleteUser(UserModel user)
        {
            var identity = await UserManager.FindByEmailAsync(user.Email);
            await UserManager.DeleteAsync(identity);
        }

        public async Task<UserModel> GetUserByEmail(string userEmail)
        {
            var user = await UserManager.FindByEmailAsync(userEmail);
            return new UserModel
            {
                Email = user.Email,
                Name = user.Name
            };
        }
    }
}
