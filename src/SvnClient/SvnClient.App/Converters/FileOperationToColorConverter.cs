using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;
using SvnClient.Backend.Models;

namespace SvnClient.App.Converters
{
    class FileOperationToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is SvnChangeFileOperation operation)
            {
                switch (operation)
                {
                    case SvnChangeFileOperation.Added:
                        return Color.FromArgb(128, 0, 255, 0);
                    case SvnChangeFileOperation.Deleted:
                        return Color.FromArgb(128, 255, 0, 0);
                    case SvnChangeFileOperation.None:
                        return Color.FromArgb(0, 0, 0, 0);
                }
            }
            return Color.FromArgb(0, 0, 0, 0);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
