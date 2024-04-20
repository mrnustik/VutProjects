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
using SvnClient.App.ViewModels;

namespace SvnClient.App.Views
{
    /// <summary>
    /// Interaction logic for SavedConnectionsView.xaml
    /// </summary>
    public partial class SavedConnectionsView : Window
    {
        public SavedConnectionsView()
        {
            InitializeComponent();
        }

        private void CloseButtonClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void OnInitialized(object sender, EventArgs e)
        {
            if (DataContext is SavedConnectionsViewModel viewModel)
            {
                viewModel.Init(this);
            }
        }
    }
}
