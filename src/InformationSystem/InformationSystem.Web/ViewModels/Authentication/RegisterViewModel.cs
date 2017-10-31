using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.ViewModel;
using DotVVM.Framework.ViewModel.Validation;
using InformationSystem.BL.Services;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.Web.ViewModels.Authentication
{
    public class RegisterViewModel : MasterPageViewModel, IValidatableObject
    {
        private UserService UserService { get; }

        public RegisterViewModel(UserService userService)
        {
            UserService = userService;
        }

        [Required]
        public string Email { get; set; }
        [Required]
        public string Password { get; set; }
        [Required]
        public string ConfirmPassword { get; set; }

        public async Task RegisterAsync()
        {
            var identityResult = await UserService.CreateUserAsync(Email, Password, UserType.Mechanic);
            if (identityResult.Succeeded)
            {
                await SignIn();
            }
            else
            {
                var modelErrors = ConvertIdentityErrorsToModelErrors(identityResult);
                Context.ModelState.Errors.AddRange(modelErrors);
                Context.FailOnInvalidModelState();
            }

            Context.RedirectToRoute("Default", allowSpaRedirect: false);
        }

        public async Task SignIn()
        {
            await UserService.SignInAsync(Email, Password);
        }

        private IEnumerable<ViewModelValidationError> ConvertIdentityErrorsToModelErrors(IdentityResult identityResult)
        {
            return identityResult.Errors.Select(identityError => new ViewModelValidationError
            {
                ErrorMessage = identityError.Description,
                PropertyPath = nameof(Password)
            });
        }

        public IEnumerable<ValidationResult> Validate(ValidationContext validationContext)
        {
            if (Password != ConfirmPassword)
            {
                yield return new ValidationResult("Passwords dont match", new[] { nameof(ConfirmPassword) });
            }
        }
    }
}

