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
    public class MainViewModel : ViewModelBase
    {

        public ObservableCollection<RepositoryViewModel> Repositories { get; set; } = new ObservableCollection<RepositoryViewModel>();
        public IMessenger Messenger { get; }
        public SvnRepository SvnRepository { get; }
        public ICommand OpenConnectionCommand { get; }
        public ICommand CloseConnectionCommand { get; }

        public MainViewModel(IMessenger messenger, SvnRepository svnRepository)
        {
            Messenger = messenger;
            SvnRepository = svnRepository;
            OpenConnectionCommand = new RelayCommand(OpenConnection);
            CloseConnectionCommand = new RelayCommand(CloseConnection);
            Messenger.Register<ConnectionOpenedMessage>(ConnectionOpenedMesssageReceived);
        }
        
        private void ConnectionOpenedMesssageReceived(ConnectionOpenedMessage connectionOpenedMessage)
        {
            var repositoryViewModel = new RepositoryViewModel(connectionOpenedMessage.Connection, SvnRepository);
            Repositories.Add(repositoryViewModel);
            repositoryViewModel.Init();
        }

        private void OpenConnection()
        {
            var window = new OpenConnectionWindow();
            window.ShowDialog();
        }

        private void CloseConnection(object obj)
        {
            var viewModel = obj as RepositoryViewModel;
            Repositories.Remove(viewModel);
        }
    }
}
