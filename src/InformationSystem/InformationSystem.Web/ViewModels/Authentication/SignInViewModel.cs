using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.ViewModel;
using DotVVM.Framework.ViewModel.Validation;
using InformationSystem.BL.Services;
using Microsoft.AspNetCore.Http.Features;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.Web.ViewModels.Authentication
{
    public class SignInViewModel : MasterPageViewModel
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
            if (Context.Query.ContainsKey("ReturnUrl"))
            {
                ReturnUrl = (string)Context.Query["ReturnUrl"];
            }
            return base.Init();
        }

        public async Task SignInAsync()
        {
            if (string.IsNullOrEmpty(SignInPassword))
            {
                ShowAlert = true;
                AlertText = "Mus�te zadat heslo.";
                AlertType = AlertDanger;
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Mus�te zadat heslo.",
                    PropertyPath = nameof(SignInPassword)
                });
                Context.FailOnInvalidModelState();
                return;
            }

            if (await userService.SignInAsync(SignInEmail, SignInPassword))
            {
                if (ReturnUrl != null)
                {
                    Context.RedirectToUrl(ReturnUrl, true);
                }
                Context.RedirectToRoute("Default");
            }
            else
            {
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Nespr�vn� u�ivatelsk� jm�no nebo heslo.",
                    PropertyPath = nameof(SignInPassword)
                });
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Nespr�vn� u�ivatelsk� jm�no nebo heslo.",
                    PropertyPath = nameof(SignInEmail)
                });
                Context.FailOnInvalidModelState();
                ShowAlert = true;
                AlertText = "Nespr�vn� u�ivatelsk� jm�no nebo heslo.";
                AlertType = AlertDanger;
            }
        }

        public async Task RegisterAsync()
        {
            if (IsRegistrationValid())
            {
                var identityResult = await userService.CreateUserAsync(RegisterName, RegisterEmail, RegisterPassword);
                if (identityResult.Succeeded)
                {
                    SignInEmail = RegisterEmail;
                    SignInPassword = RegisterPassword;
                    await SignInAsync();
                }
                else
                {
                    ShowAlert = true;
                    AlertType = AlertDanger;
                    AlertText = identityResult.Errors.FirstOrDefault()?.Description;
                }
            }
        }

        private bool IsRegistrationValid()
        {
            if (string.IsNullOrEmpty(RegisterName))
            {
                ShowAlert = true;
                AlertText = "Mus�te zadat va�e jm�no.";
                AlertType = AlertDanger;
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Mus�te zadat va�e jm�no.",
                    PropertyPath = nameof(RegisterName)
                });
                Context.FailOnInvalidModelState();
                return false;
            }

            bool isValid;
            try
            {
                var addr = new System.Net.Mail.MailAddress(RegisterEmail);
                isValid = addr.Address == RegisterEmail;
            }
            catch
            {
                ShowAlert = true;
                AlertText = "Mus�te zadat v� email.";
                AlertType = AlertDanger;
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Mus�te zadat v� email.",
                    PropertyPath = nameof(RegisterEmail)
                });
                Context.FailOnInvalidModelState();
                return false;
            }

            if (string.IsNullOrEmpty(RegisterPassword))
            {
                ShowAlert = true;
                AlertText = "Mus�te zadat heslo.";
                AlertType = AlertDanger;
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Mus�te zadat heslo.",
                    PropertyPath = nameof(RegisterPassword)
                });
                Context.FailOnInvalidModelState();
                return false;
            }

            if (RegisterPassword != RegisterConfirmPassword)
            {
                ShowAlert = true;
                AlertText = "Hesla se neshoduj�.";
                AlertType = AlertDanger;
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Hesla se neshoduj�.",
                    PropertyPath = nameof(RegisterPassword)
                });
                Context.ModelState.Errors.Add(new ViewModelValidationError
                {
                    ErrorMessage = "Hesla se neshoduj�.",
                    PropertyPath = nameof(RegisterConfirmPassword)
                });
                Context.FailOnInvalidModelState();
                return false;
            }

            return isValid;
        }

        private IEnumerable<ViewModelValidationError> ConvertIdentityErrorsToModelErrors(IdentityResult identityResult)
        {
            return identityResult.Errors.Select(identityError => new ViewModelValidationError
            {
                ErrorMessage = identityError.Description,
                PropertyPath = nameof(RegisterPassword)
            });
        }
    }
}

