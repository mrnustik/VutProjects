using System;
using System.Threading.Tasks;
using DotVVM.Framework.Controls;
using DotVVM.Framework.Runtime.Filters;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.User.Cars
{
    [Authorize]
    public class ListViewModel : MasterPageViewModel
    {
        private readonly CarService _carService;

        public ListViewModel(CarService carService)
        {
            _carService = carService;
        }

        public GridViewDataSet<CarDetailModel> Cars { get; set; }


        public async Task Delete(CarListModel car)
        {
            await _carService.DeleteCar(car.Id);
            await Cars.RequestRefreshAsync(true);
        }

        public override Task Init()
        {
            Cars = GridViewDataSet.Create(GridViewDataSetLoadDelegate, pageSize: 5);
            return base.Init();
        }

        private GridViewDataSetLoadedData<CarDetailModel> GridViewDataSetLoadDelegate(IGridViewDataSetLoadOptions gridViewDataSetLoadOptions)
        {
            var queryable = _carService.GetAllCarsByUser(UserName);

            return queryable.GetDataFromQueryable(gridViewDataSetLoadOptions);
        }
    }
}
