using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.Address;
using InformationSystem.BL.Services;

namespace InformationSystem.Web.ViewModels.User.Invoice
{
    public class AddressViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly AddressService _addressService;
        private readonly RepairService _repairService;
        private readonly UserService _userService;
        private readonly InvoiceService _invoiceService;

        public AddressViewModel(AddressService addressService, RepairService repairService, InvoiceService invoiceService, UserService userService)
        {
            _addressService = addressService;
            _repairService = repairService;
            _invoiceService = invoiceService;
            _userService = userService;
        }


        public AddressModel Address { get; set; }
        public bool SaveAddress { get; set; }
        public Guid ReparId { get; set; }

        public override async Task Init()
        {
            if (!Context.IsPostBack)
            {
                ReparId = new Guid((string)Context.Parameters["Id"]);
                var userAddress = await _addressService.GetUserAddress(UserName);
                Address = userAddress == null ? new AddressModel { UserEmail = UserName } : userAddress;
            }
            await base.Init();
        }

        public async Task GenerateInvoice()
        {
            if (IsValid(Address))
            {
                if (SaveAddress)
                {
                    await _addressService.AddAddress(Address);
                }
                var repair = await _repairService.GetRepairDetailAsync(ReparId);
                var user = await _userService.GetUserByEmail(UserName);
                var pdf = await _invoiceService.CreateInvoicePdf(repair, Address, user);
                Context.ReturnFile(pdf, "Invoice.pdf", "application/pdf");
            }
        }

        private bool IsValid(AddressModel address)
        {
            if (string.IsNullOrEmpty(address.Address))
            {
                ShowAlert = true;
                AlertText = "You have to enter valid address";
                AlertType = AlertDanger;
                return false;
            }

            if (string.IsNullOrEmpty(address.City))
            {
                ShowAlert = true;
                AlertText = "You have to enter city address";
                AlertType = AlertDanger;
                return false;
            }

            if (string.IsNullOrEmpty(address.ZipCode))
            {
                ShowAlert = true;
                AlertText = "You have to enter zip code.";
                AlertType = AlertDanger;
                return false;
            }

            if (string.IsNullOrEmpty(address.State))
            {
                ShowAlert = true;
                AlertText = "You have to enter state";
                AlertType = AlertDanger;
                return false;
            }

            return true;
        }




    }
}

