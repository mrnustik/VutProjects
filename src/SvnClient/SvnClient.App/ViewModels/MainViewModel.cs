using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using SvnClient.App.Command;
using SvnClient.App.Views;
using SvnClient.Backend;
using SvnClient.Backend.Messages;

namespace SvnClient.App.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        private RepositoryViewModel _selectedRepository;
        private ObservableCollection<RepositoryViewModel> _repositories;

        public ObservableCollection<RepositoryViewModel> Repositories
        {
            get
            {
                if (_repositories == null)
                {
                    _repositories = new ObservableCollection<RepositoryViewModel>();
                    var itemsView = (IEditableCollectionView) CollectionViewSource.GetDefaultView(_repositories);
                    itemsView.NewItemPlaceholderPosition = NewItemPlaceholderPosition.AtEnd;
                }
                return _repositories;
            }
        }

        public IMessenger Messenger { get; }
        public SvnRepository SvnRepository { get; }
        public ICommand OpenConnectionCommand { get; }
        public ICommand CloseConnectionCommand { get; }

        public RepositoryViewModel SelectedRepository
        {
            get { return _selectedRepository; }
            set
            {
                if (_selectedRepository == value) return;
                _selectedRepository = value;
                OnPropertyChanged();
            }
        }

        public MainViewModel(IMessenger messenger, SvnRepository svnRepository)
        {
            Messenger = messenger;
            SvnRepository = svnRepository;
            OpenConnectionCommand = new RelayCommand(OpenConnection);
            CloseConnectionCommand = new RelayCommand(CloseConnection);
            Messenger.Register<ConnectionOpenedMessage>(ConnectionOpenedMesssageReceived);
        }

        public void Init()
        {
            OpenConnection();
        }

        private void ConnectionOpenedMesssageReceived(ConnectionOpenedMessage connectionOpenedMessage)
        {
            var repositoryViewModel = new RepositoryViewModel(connectionOpenedMessage.Connection, SvnRepository);
            Repositories.Add(repositoryViewModel);
            repositoryViewModel.Init();
            SelectedRepository = repositoryViewModel;
        }

        private void OpenConnection()
        {
            var window = new SavedConnectionsView();
            window.Show();
        }

        private void CloseConnection(object obj)
        {
            var viewModel = obj as RepositoryViewModel;
            Repositories.Remove(viewModel);
        }
    }
}
