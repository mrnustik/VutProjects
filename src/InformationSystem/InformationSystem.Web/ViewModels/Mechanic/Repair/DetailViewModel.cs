using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Controls;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Material;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.Mechanic.Repair
{
    public class DetailViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly RepairService _repairService;
        private readonly MaterialService _materialService;
        public DetailViewModel(RepairService repairService, MaterialService materialService)
        {
            _repairService = repairService;
            _materialService = materialService;
        }

        public List<MaterialModel> Materials { get; set; }
        public RepairDetailModel Repair { get; set; }

        public GridViewDataSet<UsedMaterialModel> UsedMaterials { get; set; }

        public override async Task Init()
        {
            if (Context.IsPostBack == false)
            {
                var repairId = new Guid((string) Context.Parameters["Id"]);
                Repair = await _repairService.GetRepairDetailAsync(repairId);
                UsedMaterials = GridViewDataSet.Create(options =>
                {
                    var queryable = _materialService.GetUsedMaterialsForRepair(repairId);
                    return queryable.GetDataFromQueryable(options);
                }, 1000);
                UsedMaterials.RowEditOptions.PrimaryKeyPropertyName = "Id";
                Materials = _materialService.GetAllMaterials().ToList();
            }
            await base.Init();
        }

        public void CancelEdit()
        {
            UsedMaterials.RowEditOptions.EditRowId = null;
            UsedMaterials.RequestRefresh(true);
        }

        public async Task BeginEdit(UsedMaterialModel material)
        {
            UsedMaterials.RowEditOptions.EditRowId = material.Id;
            await UsedMaterials.RequestRefreshAsync(true);
        }
        
        public async Task AddOrUpdateMaterial(UsedMaterialModel material)
        {
            await _materialService.AddOrUpdateMaterialToRepair(Repair.Id, material);
            UsedMaterials.RowEditOptions.EditRowId = null;
            await UsedMaterials.RequestRefreshAsync(true);
        }

        public async Task RemoveMaterial(UsedMaterialModel material)
        {
            await _materialService.RemoveMaterialFromRepair(Repair.Id, material);
            UsedMaterials.Items.Remove(material);
            UsedMaterials.RowEditOptions.EditRowId = null;
            await UsedMaterials.RequestRefreshAsync(true);
        }

        public async Task MarkDone()
        {
            Repair.Done = true;
            await _repairService.UpdateRepair(Repair);
            UsedMaterials.RowEditOptions.EditRowId = null;
            UsedMaterials.RequestRefresh(true);
        }

        public async Task MarkUndone()
        {
            Repair.Done = false;
            await _repairService.UpdateRepair(Repair);
        }

        public void AddMaterial()
        {
            var materialModel = new UsedMaterialModel {Id = new Guid(), Ammount = 1.0};
            UsedMaterials.Items.Add(materialModel);
            UsedMaterials.RowEditOptions.EditRowId = materialModel.Id;
            UsedMaterials.RequestRefresh(true);
        }
    }
}

