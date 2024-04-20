using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Input;
using TemperatureStation.App.Models;
using TemperatureStation.App.Services;
using Xamarin.Forms;

namespace TemperatureStation.App.ViewModels
{
    public class MainPageViewModel : INotifyPropertyChanged
    {
        private readonly StationAccessService _stationAccessService;
        private readonly Timer _timer;
        private List<MeasureModel> _measurements;
        private DateTime _baseDate;
        private Dictionary<string, TemperatureUnit> _temperatureUnits = new Dictionary<string, TemperatureUnit>
        {
            { "Celsius", TemperatureUnit.Celsius},
            {"Fahrenheit", TemperatureUnit.Fahrenheit },
            {"Kelvin", TemperatureUnit.Kelvin }
        };

        private int _selectedUnit;
        private string _refreshRate = "2000";

        public MainPageViewModel(StationAccessService stationAccessService)
        {
            _stationAccessService = stationAccessService;
            _timer = new Timer {AutoReset = true};
            _timer.Elapsed += RefreshTimerElapsed;
            _timer.Interval = 2000;
            _timer.Start();
            RefreshCommand = new Command(Refresh);
            SetRefreshRateCommand = new Command(SetRefreshRateAsync);
        }

        private void RefreshTimerElapsed(object sender, ElapsedEventArgs elapsedEventArgs)
        {
            Refresh();
        }

        private async void SetRefreshRateAsync(object refreshRate)
        {
            if (refreshRate is string rateString)
            {
                if (long.TryParse(rateString, out var refreshTime))
                {
                    await _stationAccessService.UpdateRefreshTime(refreshTime);
                    _timer.Interval = refreshTime;
                    _timer.Stop();
                    _timer.Start();
                }
            }
        }

        public ICommand RefreshCommand { get; }
        public ICommand SetRefreshRateCommand { get; }

        private async void Refresh()
        {
            try
            {
                RefreshRate = (await _stationAccessService.GetRefreshTime()).ToString();
                BaseDate = await _stationAccessService.GetBaseDate();
                Measurements = (await _stationAccessService.GetHistory(BaseDate, _temperatureUnits.Values.ElementAt(SelectedUnit))).OrderByDescending(m => m.Timestamp).ToList();
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

        public int SelectedUnit
        {
            get => _selectedUnit;
            set
            {
                if (_selectedUnit == value) return;
                _selectedUnit = value;
                Refresh();
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

        public string RefreshRate
        {
            get { return _refreshRate; }
            set
            {
                if (_refreshRate == value) return;
                _refreshRate = value;
                OnPropertyChanged();
            }
        }
    }
}
