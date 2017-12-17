using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TemperatureStation.App.Models;
using TemperatureStation.App.ViewModels;
using Xamarin.Forms;

namespace TemperatureStation.App
{
	public partial class MainPage : ContentPage
	{
		public MainPage()
		{
			InitializeComponent();
		}

        public void MainPageAppearing(object sender, EventArgs e)
        {
            if (BindingContext is MainPageViewModel viewModel)
            {
                viewModel.Initialize(this);
            }
        }

	    public void ShowAlert(string message)
	    {
	        DisplayAlert("An error occured", message, "OK");
	    }
	}
}
