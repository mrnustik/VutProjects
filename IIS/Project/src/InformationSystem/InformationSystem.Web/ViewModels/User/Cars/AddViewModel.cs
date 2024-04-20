using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
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
            if (IsValid(Car))
            {
                await _carService.CreateCar(Car, UserName);
                Context.RedirectToRoute("User_Cars_List");
            }
        }

        private bool IsValid(CarDetailModel car)
        {

            if (string.IsNullOrEmpty(car.Name))
            {
                ShowAlert = true;
                AlertText = "Jméno vozidla je poviné.";
                AlertType = AlertDanger;
                return false;
            }
            
            if (string.IsNullOrEmpty(car.CompanyName))
            {
                ShowAlert = true;
                AlertText = "Výrobce vozidla je poviný.";
                AlertType = AlertDanger;
                return false;
            }

            if (string.IsNullOrEmpty(car.TypeName))
            {
                ShowAlert = true;
                AlertText = "Typ vozidla je poviný.";
                AlertType = AlertDanger;
                return false;
            }

            if (string.IsNullOrEmpty(car.CarIdenitification))
            {
                ShowAlert = true;
                AlertText = "Poznávací značka je poviná.";
                AlertType = AlertDanger;
                return false;
            }

            return true;
        }
    }
}

