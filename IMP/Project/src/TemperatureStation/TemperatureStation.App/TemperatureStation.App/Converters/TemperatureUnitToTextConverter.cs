using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using TemperatureStation.App.Models;
using Xamarin.Forms;

namespace TemperatureStation.App.Converters
{
    public class TemperatureUnitToTextConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is int intValue)
            {
                var unit = (TemperatureUnit) intValue;
                switch (unit)
                {
                    case TemperatureUnit.Celsius:
                        return "°C";
                    case TemperatureUnit.Fahrenheit:
                        return "°F";
                    case TemperatureUnit.Kelvin:
                        return "K";
                }
            }
            return string.Empty;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
