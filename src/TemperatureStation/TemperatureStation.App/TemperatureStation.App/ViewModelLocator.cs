using System;
using System.Collections.Generic;
using System.Text;
using TemperatureStation.App.Services;
using TemperatureStation.App.ViewModels;

namespace TemperatureStation.App
{
    public class ViewModelLocator
    {
        private readonly StationAccessService _stationAccessService = new StationAccessService();

        public MainPageViewModel MainPageViewModel => CreateMainPageViewModel();

        private MainPageViewModel CreateMainPageViewModel()
        {
            return new MainPageViewModel(_stationAccessService);
        }
    }
}
