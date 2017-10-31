using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.Authentication
{
    public class SignInViewModel : MasterPageViewModel
    {
        private readonly UserService _userService;

        public SignInViewModel(UserService userService)
        {
            _userService = userService;
        }
        
        [Required]
        public string Email { get; set; }
        [Required]
        public string Password { get; set; }

        public async Task SingInAsync()
        {
            if (await _userService.SignInAsync(Email, Password))
            {
                Context.RedirectToRoute("Default");
            }
        }

    }
}

