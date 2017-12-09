using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using SvnClient.App.Command;
using SvnClient.App.Views;
using SvnClient.Backend;
using SvnClient.Backend.Messages;
using SvnClient.Backend.Stores;

namespace SvnClient.App.ViewModels 
{
    public class SavedConnectionsViewModel : ViewModelBase
    {
        private readonly IMessenger _messenger;
        private readonly SavedConnectionsStore _connectionsStore;
        private Window _window;
        private ObservableCollection<SvnConnection> _connections = new ObservableCollection<SvnConnection>();

        public SavedConnectionsViewModel(IMessenger messenger, SavedConnectionsStore connectionsStore)
        {
            _messenger = messenger;
            _connectionsStore = connectionsStore;
            _messenger.Register<ConnectionAddedMessage>(ConnectionAdded);
            AddCommand = new RelayCommand(AddConnection);
            RemoveCommand = new RelayCommand(RemoveConnection);
            UpdateCommand = new RelayCommand(UpdateConnection);
            OpenCommand = new RelayCommand(OpenConnection);
        }

        private void ConnectionAdded(ConnectionAddedMessage message)
        {
            var connection = message.Connection;
            if (connection.Id != Guid.Empty)
            {
                _connectionsStore.Update(connection);
            }
            else
            {
                connection.Id = Guid.NewGuid();
                _connectionsStore.Store(connection);
            }
            LoadData();
        }

        public void Init(Window window)
        {
            _window = window;
            LoadData();
        }

        public ObservableCollection<SvnConnection> Connections
        {
            get { return _connections; }
            set
            {
                if (_connections == value) return;
                _connections = value;
                OnPropertyChanged();
            }
        }

        private void OpenConnection(object o)
        {
            if (o is SvnConnection connection)
            {
                var message = new ConnectionOpenedMessage(connection);
                _messenger.Send(message);
                _window.Close();
            }
        }

        private void UpdateConnection(object o)
        {
            if (o is SvnConnection connection)
            {
                var window = new OpenConnectionWindow();
                if (window.DataContext is OpenConnectionViewModel viewModel)
                {
                    viewModel.Name = connection.Name;
                    viewModel.Url = connection.RemoteUri.AbsoluteUri;
                }
                window.ShowDialog();
            }
        }

        private void RemoveConnection(object o)
        {
            if (o is SvnConnection connection)
            {
                _connectionsStore.Delete(connection);
                LoadData();
            }
        }

        private void LoadData()
        {
            Connections = new ObservableCollection<SvnConnection>(_connectionsStore.GetAll());_connectionsStore.GetAll();
        }

        private void AddConnection()
        {
            var window = new OpenConnectionWindow();
            window.ShowDialog();
        }

        public ICommand AddCommand { get; }
        public ICommand RemoveCommand { get; }
        public ICommand UpdateCommand { get; }
        public ICommand OpenCommand { get;  }

    }
}
