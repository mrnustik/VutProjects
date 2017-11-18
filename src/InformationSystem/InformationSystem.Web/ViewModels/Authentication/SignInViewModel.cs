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
    public class SignInViewModel : MasterPageViewModel, IValidatableObject
    {
        private readonly UserService userService;

        public SignInViewModel(UserService userService)
        {
            this.userService = userService;
        }
        

        [Bind(Direction.ClientToServer)]
        public string SignInEmail { get; set; }


        [Bind(Direction.ClientToServer)]
        public string SignInPassword { get; set; }

        [Bind(Direction.ClientToServer)]
        public string RegisterName { get; set; }
        [Bind(Direction.ClientToServer)]
        public string RegisterEmail { get; set; }

        [Bind(Direction.ClientToServer)]
        public string RegisterPassword { get; set; }

        [Bind(Direction.ClientToServer)]
        public string RegisterConfirmPassword { get; set; }

        public string ReturnUrl { get; set; } = "/";

        public override Task Init()
        {
            if(Context.Query.ContainsKey("ReturnUrl")){
                ReturnUrl = (string)Context.Query["ReturnUrl"];
            }
            return base.Init();
        }

        public async Task SignInAsync()
        {
            if (await userService.SignInAsync(SignInEmail, SignInPassword))
            {
                if(ReturnUrl != null){
                    Context.RedirectToUrl(ReturnUrl, true);
                }
                Context.RedirectToRoute("Default");
            }
            else 
            {
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Invalid user name or password",
                    PropertyPath = nameof(SignInPassword)
                });
            }
        }

        public async Task RegisterAsync()
        {
            var identityResult = await userService.CreateUserAsync(RegisterName,RegisterEmail, RegisterPassword);
            if (identityResult.Succeeded)
            {
                SignInEmail = RegisterEmail;
                SignInPassword = RegisterPassword;
                await SignInAsync();
            }
            else
            {
                var modelErrors = ConvertIdentityErrorsToModelErrors(identityResult);
                Context.ModelState.Errors.AddRange(modelErrors);
                Context.FailOnInvalidModelState();
            }
        }

        private IEnumerable<ViewModelValidationError> ConvertIdentityErrorsToModelErrors(IdentityResult identityResult)
        {
            return identityResult.Errors.Select(identityError => new ViewModelValidationError
            {
                ErrorMessage = identityError.Description,
                PropertyPath = nameof(RegisterPassword)
            });
        }

        public IEnumerable<ValidationResult> Validate(ValidationContext validationContext)
        {
            if (RegisterPassword != RegisterConfirmPassword)
            {
                yield return new ValidationResult("Passwords dont match", new[] { nameof(RegisterConfirmPassword) });
            }
        }
    }
}

