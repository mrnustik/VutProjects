using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using SvnClient.Backend.Models;

namespace SvnClient.App.Converters
{
    public class NodeTypeToImageConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is SvnNodeType change)
            {
                switch (change)
                {
                    case SvnNodeType.File:
                        return "../Resources/IconFile.png";
                    case SvnNodeType.Directory:
                        return "../Resources/IconFolder.png";
                }
            }
            return "../Resources/IconFile.png";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}