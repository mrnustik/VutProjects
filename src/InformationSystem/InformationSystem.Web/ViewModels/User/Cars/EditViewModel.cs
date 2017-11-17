using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.User.Cars
{
    public class EditViewModel : MasterPageViewModel
    {
        private readonly CarService _carService;
        public EditViewModel(CarService carService)
        {
            _carService = carService;
        }

        public CarDetailModel Car { get; set; }

        public override async Task Init()
        {
            var carId = Guid.Parse((string) Context.Parameters["Id"]);
            Car = await _carService.GetCarById(carId);
            await base.Init();
        }
    }
}

