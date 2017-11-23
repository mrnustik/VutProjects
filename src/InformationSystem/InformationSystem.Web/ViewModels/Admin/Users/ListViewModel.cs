using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DotVVM.Framework.Controls;
using DotVVM.Framework.Runtime.Filters;
using DotVVM.Framework.ViewModel;
using InformationSystem.BL.Models.User;
using InformationSystem.BL.Services;


namespace InformationSystem.Web.ViewModels.Admin.Users
{
    [Authorize(Roles = "admin")]
    public class ListViewModel : InformationSystem.Web.ViewModels.MasterPageViewModel
    {
        private readonly UserService _userService;

        public ListViewModel(UserService userService)
        {
            _userService = userService;
        }

        public string[] Roles { get; set; } =
        {
            "admin",
            "Mechanic",
            "Regular"
        };

        public GridViewDataSet<UserModel> Users { get; set; }

        public override Task Init()
        {
            Users = GridViewDataSet.Create(GridViewDataSetLoadDelegate, 10);
            Users.RowEditOptions = new RowEditOptions {PrimaryKeyPropertyName = "Email"};
            return base.Init();
        }

        public void Edit(UserModel user)
        {
            Users.RowEditOptions.EditRowId = user.Email;
        }

        public async Task Save(UserModel user)
        {
            Users.RowEditOptions.EditRowId = null;
            var identity = await _userService.UserManager.FindByNameAsync(user.Email);
            var roles = await _userService.UserManager.GetRolesAsync(identity);
            await _userService.UserManager.RemoveFromRolesAsync(identity, roles);
            await _userService.UserManager.AddToRoleAsync(identity, user.Role);
            await Users.RequestRefreshAsync(true);
        }


        public void ShowUserDeleteDialog(UserModel user)
        {
            DialogText = $"Opravdu chcete deaktivovat uživatel {user.Email}?";
            DialogUser = user;
            DialogShown = true;
        }

        public async Task DeleteUser()
        {
            await _userService.DeleteUser(DialogUser);
            Users.RequestRefresh(true);
            DialogShown = false;
        }

        public void HideDialog()
        {
            DialogShown = false;
        }

        public Task CancelEdit()
        {
            Users.RowEditOptions.EditRowId = null;
            return Users.RequestRefreshAsync(true);
        }

        private async Task<GridViewDataSetLoadedData<UserModel>> GridViewDataSetLoadDelegate(IGridViewDataSetLoadOptions gridViewDataSetLoadOptions)
        {
            var users = await _userService.GetAllUsers();

            return users.GetDataFromQueryable(gridViewDataSetLoadOptions);
        }

        public bool DialogShown { get; set; } = false;
        public string DialogHeader { get; set; } = "Odebrat uživatele";
        public string DialogText { get; set; } = "";
        public UserModel DialogUser { get; set; } = new UserModel();
        public string YesButtonText { get; set; } = "Ano";
        public string NoButtonText { get; set; } = "Ne";

    }
}

