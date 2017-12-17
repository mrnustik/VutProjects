using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using TemperatureStation.App.Models;
using TemperatureStation.App.Services;
using Xamarin.Forms;

namespace TemperatureStation.App.ViewModels
{
    public class MainPageViewModel : INotifyPropertyChanged
    {
        private readonly StationAccessService _stationAccessService;
        private List<MeasureModel> _measurements;
        private DateTime _baseDate;

        public MainPageViewModel(StationAccessService stationAccessService)
        {
            _stationAccessService = stationAccessService;
            RefreshCommand = new Command(Refresh);
        }
        public ICommand RefreshCommand { get; }

        private async void Refresh()
        {
            try
            {
                BaseDate = await _stationAccessService.GetBaseDate();
                Measurements = (await _stationAccessService.GetHistory(BaseDate)).ToList();
            }
            catch (Exception e)
            {
                MainPage.ShowAlert("Can't connect to station. Are you sure it is on the same network?");
            }
        }


        public List<MeasureModel> Measurements
        {
            get => _measurements;
            set
            {
                if (_measurements == value) return;
                _measurements = value;
                OnPropertyChanged();
            }
        }

        public DateTime BaseDate
        {
            get => _baseDate;
            set
            {
                if(_baseDate == value) return;
                _baseDate = value;
                OnPropertyChanged();
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public void Initialize(MainPage mainPage)
        {
            this.MainPage = mainPage;
            Refresh();
        }

        public MainPage MainPage { get; set; }
    }
}
