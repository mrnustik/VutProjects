using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Runtime.Filters;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.User.Cars
{
    [Authorize]
    public class AddViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly CarService _carService;
        public AddViewModel(CarService carService)
        {
            _carService = carService;
        }

        public CarDetailModel Car { get; set; } = new CarDetailModel();

        public async Task Save()
        {
            await _carService.CreateCar(Car, UserName);
            Context.RedirectToRoute("User_Cars_List");
        }

    }
}

