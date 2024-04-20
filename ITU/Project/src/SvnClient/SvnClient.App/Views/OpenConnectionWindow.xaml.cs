using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace SvnClient.App.Views
{
    /// <summary>
    /// Interaction logic for OpenConnectionWindow.xaml
    /// </summary>
    public partial class OpenConnectionWindow : Window
    {
        public OpenConnectionWindow()
        {
            InitializeComponent();
        }

        private void CloseButtonClicked(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
