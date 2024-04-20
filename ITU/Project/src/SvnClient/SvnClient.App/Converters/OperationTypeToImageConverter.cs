using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using SvnClient.Backend.Models;

namespace SvnClient.App.Converters
{
    public class OperationTypeToImageConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is SvnChangeType change)
            {
                switch (change)
                {
                    case SvnChangeType.Add:
                        return "../Resources/IconAdd.png";
                    case SvnChangeType.Modify:
                        return "../Resources/IconEdit.png";
                    case SvnChangeType.Delete:
                        return "../Resources/IconDelete.png";
                }
            }
            return "../Resources/IconAdd.png";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}