using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using SvnClient.App.Command;
using SvnClient.Backend;
using SvnClient.Backend.Messages;

namespace SvnClient.App.ViewModels
{
    public class OpenConnectionViewModel : ViewModelBase
    {
        private bool _showProgress;
        public string Name { get; set; }
        public string Url { get; set; }

        public bool ShowProgress
        {
            get { return _showProgress; }
            set
            {
                if (_showProgress == value) return;
                _showProgress = value;
                OnPropertyChanged();
            }
        }

        public ICommand OpenCommand { get; }

        public IMessenger Messenger { get; }

        

        public OpenConnectionViewModel(IMessenger messenger)
        {
            Messenger = messenger;
            ShowProgress = false;
            OpenCommand = new RelayCommand(OpenConnection);
        }

        private async void OpenConnection(object window)
        {
            try
            {
                ShowProgress = true;
                var svnConnection = await Task.Run(() =>
                {
                    var connection = SvnConnection.OpenConnection(Name, Url);
                    return connection;
                });
                
                if (svnConnection != null)
                {
                    var message = new ConnectionOpenedMessage(svnConnection);
                    Messenger.Send(message);
                    (window as Window)?.Close();
                }
                else
                {
                    MessageBox.Show("Exception occured when downloading remote repository. Is the url right?");
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Exception occured when downloading remote repository. Is the url right?");
            }
            ShowProgress = false;
        }
    }
}
