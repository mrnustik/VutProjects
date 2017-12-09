using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SvnClient.App.ViewModels;
using SvnClient.Backend;
using SvnClient.Backend.Stores;

namespace SvnClient.App
{
    class ViewModelLocator
    {
        private readonly IMessenger _messenger = new Messenger();
        private readonly SavedConnectionsStore _connectionsStore = new SavedConnectionsStore();
        private readonly SvnRepository _svnRepository = new SvnRepository();

        public MainViewModel MainViewModel => CreateMainViewModel();
        public OpenConnectionViewModel OpenConnectionViewModel => CreateOpenConnectionViewModel();

        public SavedConnectionsViewModel SavedConnectionsViewModel => CreateSavedConnectionsViewModel();

        private SavedConnectionsViewModel CreateSavedConnectionsViewModel()
        {
            return new SavedConnectionsViewModel(_messenger, _connectionsStore);
        }

        private OpenConnectionViewModel CreateOpenConnectionViewModel()
        {
            return new OpenConnectionViewModel(_messenger);
        }

        private MainViewModel CreateMainViewModel()
        {
            return new MainViewModel(_messenger, _svnRepository);
        }
    }
}
