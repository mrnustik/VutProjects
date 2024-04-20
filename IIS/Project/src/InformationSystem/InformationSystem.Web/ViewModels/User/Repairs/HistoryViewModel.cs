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
        private readonly InvoiceService _invoiceService;
        private readonly UserService _userService;
        public HistoryViewModel(RepairService repairService, InvoiceService invoiceService, UserService userService)
        {
            _repairService = repairService;
            _invoiceService = invoiceService;
            _userService = userService;
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

        public async Task GenerateInvoice(RepairListModel repairListModel)
        {
            if (await _invoiceService.HasStoredInvoice(repairListModel.Id))
            {
                var repair = await _repairService.GetRepairDetailAsync(repairListModel.Id);
                var user = await _userService.GetUserByEmail(UserName);
                var pdf = await _invoiceService.CreateInvoicePdf(repair, null, user);
                Context.ReturnFile(pdf, "Invoice.pdf", "application/pdf");
            }
            else
            {
                Context.RedirectToRoute("User_Invoice_Address", new { Id = repairListModel.Id});
            }
        }
    }
}

