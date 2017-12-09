using System.Windows;

namespace SvnClient.App.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void CloseMenuItemClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
