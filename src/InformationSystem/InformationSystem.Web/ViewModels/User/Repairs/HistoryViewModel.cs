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

namespace InformationSystem.Web.ViewModels.User.Repairs
{
    [Authorize]
    public class HistoryViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly RepairService _repairService;
        public HistoryViewModel(RepairService repairService)
        {
            _repairService = repairService;
        }

        public GridViewDataSet<RepairListModel> Repairs { get; set; }

        public override Task Init()
        {
            Repairs = GridViewDataSet.Create(GridViewDataSetLoadDelegate, pageSize: 10);
            return base.Init();
        }

        private GridViewDataSetLoadedData<RepairListModel> GridViewDataSetLoadDelegate(IGridViewDataSetLoadOptions gridViewDataSetLoadOptions)
        {
            var queryable = _repairService.GetAllRepairsByUser(UserName);

            return queryable.GetDataFromQueryable(gridViewDataSetLoadOptions);
        }
    }
}

