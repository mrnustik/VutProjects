using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Hosting;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Services;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.Web.ViewModels
{
    public class MasterPageViewModel : DotvvmViewModelBase
    {

        public string UserName => Context.GetAuthentication().Context?.User?.Identity?.Name;
        public string UserRole => Context.GetAuthentication().Context.User?.Claims.FirstOrDefault( c=> c.Type == ClaimTypes.Role)?.Value;
       

        public async Task SignOutAsync()
        {
            try
            {
                await Context.GetAuthentication().SignOutAsync(IdentityConstants.ApplicationScheme);
            }
            catch
            {
            }
            Context.RedirectToRoute("Default");
        }
    }
}
