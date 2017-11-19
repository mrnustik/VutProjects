using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Controls;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Material;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.Mechanic.Materials
{
    public class ListViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly MaterialService _materialService;
        public ListViewModel(MaterialService materialService)
        {
            _materialService = materialService;
        }

        public GridViewDataSet<MaterialModel> Materials { get; set; }

        public override Task Init()
        {
            if (Context.IsPostBack == false)
            {
                Materials = GridViewDataSet.Create(GridViewDataSetLoadDelegate, pageSize: 10);
                Materials.RowEditOptions.PrimaryKeyPropertyName = "Id";
            }
            return base.Init();
        }

        public void AddMaterial()
        {
            var material = new MaterialModel() {Id = Guid.NewGuid()};
            Materials.Items.Add(material);
            Materials.RowEditOptions.EditRowId = material.Id;
            Materials.RequestRefresh(true);
        }

        public async Task RemoveMaterial(MaterialModel material)
        {
            await _materialService.RemoveMaterial(material);
            await Materials.RequestRefreshAsync(true);
        }

        public async Task SaveMaterial(MaterialModel material)
        {
            await _materialService.AddOrUpdateMaterial(material);
            Materials.RowEditOptions.EditRowId = null;
            await Materials.RequestRefreshAsync(true);
        }

        public void BeginEdit(MaterialModel material)
        {
            Materials.RowEditOptions.EditRowId = material.Id;
            Materials.RequestRefresh(true);
        }

        public void CancelEdit()
        {
            Materials.RowEditOptions.EditRowId = null;
            Materials.RequestRefresh(true);
        }

        private GridViewDataSetLoadedData<MaterialModel> GridViewDataSetLoadDelegate(IGridViewDataSetLoadOptions gridViewDataSetLoadOptions)
        {
            var queryable = _materialService.GetAllMaterials();
            return queryable.GetDataFromQueryable(gridViewDataSetLoadOptions);
        }
    }
}

