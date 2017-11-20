using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Runtime.Filters;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Models.User;
using InformationSystem.BL.Services;
using InformationSystem.DAL.Entities;

namespace InformationSystem.Web.ViewModels.User.Repairs
{
    [Authorize]
    public class OrderRepairViewModel : MasterPageViewModel
    {
        private readonly CarService _carService;
        private readonly RepairService _repairService;
        public OrderRepairViewModel(CarService carService, RepairService repairService)
        {
            _carService = carService;
            _repairService = repairService;
        }

        public RepairDetailModel Repair { get; set; } = new RepairDetailModel();
        public List<CarDetailModel> Cars { get; set; } = new List<CarDetailModel>();

        public string SelectedDate { get; set; } = (DateTime.Now + TimeSpan.FromHours(24)).ToString("yyyy-MM-dd");
        public string SelectedHour { get; set; } = "08:00";
        public List<string> OpeningHours { get; set; }
        public List<UserModel> Mechanics { get; set; }

        public List<RepairTypeString> Types { get; set; } = new List<RepairTypeString>
        {
            new RepairTypeString
            {
                RepairType = RepairType.Body,
                String = "Body"
            },
            new RepairTypeString
            {
                RepairType = RepairType.Motor,
                String = "Engine"
            }
        };

        public override async Task Init()
        {
            if (!Context.IsPostBack)
            {
                Cars = _carService.GetAllCarsByUser(UserName).ToList();
                OpeningHours = Enumerable.Range(8, 9).Select(a => $"{a:D2}:00").ToList();
                await ReservationTimeChanged();
                if (Context.Parameters.ContainsKey("CarId"))
                {
                    var guid = new Guid((string)Context.Parameters["CarId"]);
                    Repair.Car = Cars.FirstOrDefault(c => c.Id == guid);
                }
            }
            await base.Init();
        }

        public async Task ReservationTimeChanged()
        {
            var datetime = ParseSelectedDate();
            Mechanics = (await _repairService.FindAvailableMechanics(datetime)).ToList();
            Repair.Mechanic = null;
        }

        private DateTime ParseSelectedDate()
        {
            var date = DateTime.Parse(SelectedDate);
            var time = TimeSpan.Parse(SelectedHour);
            var datetime = date + time;
            return datetime;
        }

        public async Task Save()
        {
            var date = ParseSelectedDate();
            Repair.ReservationDate = date;
            await _repairService.CreateRepairOrder(Repair);
            Context.RedirectToRoute("Default");
        }

    }

    public class RepairTypeString
    {
        public string String { get; set; }
        public RepairType RepairType { get; set; }
    }
}

