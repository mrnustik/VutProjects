using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DotVVM.Framework.Runtime.Filters;
using DotVVM.Framework.ViewModel;


namespace InformationSystem.Web.ViewModels.Mechanic
{
    [Authorize(Roles = "Mechanic")]
    public class TestViewModel : MasterPageViewModel
    {
            
    }
}

