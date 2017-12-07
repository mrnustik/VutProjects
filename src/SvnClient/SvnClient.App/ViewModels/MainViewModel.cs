using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using SvnClient.App.Command;
using SvnClient.App.Views;
using SvnClient.Backend;
using SvnClient.Backend.Messages;

namespace SvnClient.App.ViewModels
{
    public class RepositoryViewModel : ViewModelBase
    {
        private readonly SvnConnection _connection;

        public RepositoryViewModel(SvnConnection connection)
        {
            _connection = connection ?? throw new ArgumentNullException(nameof(connection));
        }

        public string Name => _connection.Name;

    }
    
    public class MainViewModel : ViewModelBase
    {

        public ObservableCollection<RepositoryViewModel> Repositories { get; set; } = new ObservableCollection<RepositoryViewModel>();
        public IMessenger Messenger { get; }
        public ICommand OpenConnectionCommand { get; }

        public MainViewModel(IMessenger messenger)
        {
            Messenger = messenger;
            OpenConnectionCommand = new RelayCommand(OpenConnection);
            Messenger.Register<ConnectionOpenedMessage>(ConnectionOpenedMesssageReceived);
        }
        
        private void ConnectionOpenedMesssageReceived(ConnectionOpenedMessage connectionOpenedMessage)
        {
            Repositories.Add(new RepositoryViewModel(connectionOpenedMessage.Connection));
        }

        private void OpenConnection()
        {
            var window = new OpenConnectionWindow();
            window.ShowDialog();
        }
    }
}
