using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Controls;
using DotVVM.Framework.Runtime.Filters;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.Mechanic.Repair
{
    [Authorize(Roles = "Mechanic")]
    public class ListViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly RepairService _repairService;
        public ListViewModel(RepairService repairService)
        {
            _repairService = repairService;
        }

        public GridViewDataSet<RepairListModel> Repairs { get; set; }
        public GridViewFilterPlacement Placement { get; set; }
        public bool ShowOnlyTodays { get; set; }
        public bool HideDoneWork { get; set; }

        public override Task Init()
        {
            if (!Context.IsPostBack)
            {
                Repairs = GridViewDataSet.Create(GridViewDataSetLoadDelegate, pageSize: 10);
                Placement = GridViewFilterPlacement.HeaderRow;
            }
            return base.Init();
        }

        private GridViewDataSetLoadedData<RepairListModel> GridViewDataSetLoadDelegate(IGridViewDataSetLoadOptions gridViewDataSetLoadOptions)
        {
            var queryable = _repairService.GetAllRepairsForMechanic(UserName);
            if (ShowOnlyTodays)
            {
                var tomorrow = DateTime.Today + TimeSpan.FromHours(24);
                queryable = queryable.Where(r =>
                    r.ReservationDate > DateTime.Today && r.ReservationDate < tomorrow);
            }
            if (HideDoneWork)
            {
                queryable = queryable.Where(r => r.Done == false);
            }
            return queryable.GetDataFromQueryable(gridViewDataSetLoadOptions);
        }

        public async Task ReloadDataSet()
        {
            Repairs = GridViewDataSet.Create(GridViewDataSetLoadDelegate, pageSize: 10);
            
        }
    }
}

