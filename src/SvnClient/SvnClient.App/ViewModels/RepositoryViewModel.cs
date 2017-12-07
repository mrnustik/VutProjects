using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Threading.Tasks;
using SvnClient.App.Annotations;
using SvnClient.Backend;
using SvnClient.Backend.Models;

namespace SvnClient.App.ViewModels
{
    public class RepositoryViewModel : ViewModelBase
    {
        private readonly SvnRepository _svnRepository;
        private readonly SvnConnection _connection;
        private readonly BackgroundWorker _backgroundWorker = new BackgroundWorker();
        private SvnCommitModel _selectedCommit;
        private bool _showProgress;
        private ObservableCollection<SvnCommitModel> _commitList;

        public RepositoryViewModel(SvnConnection connection, [NotNull] SvnRepository svnRepository)
        {
            _svnRepository = svnRepository ?? throw new ArgumentNullException(nameof(svnRepository));
            _connection = connection ?? throw new ArgumentNullException(nameof(connection));
        }

        public string Name => _connection.Name;

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

        public SvnCommitModel SelectedCommit
        {
            get { return _selectedCommit; }
            set
            {
                if (_selectedCommit == value) return;
                _selectedCommit = value; 
                OnPropertyChanged();
            }
        }

        public ObservableCollection<SvnCommitModel> CommitList
        {
            get { return _commitList; }
            set {
                if (_commitList == value) return;
                _commitList = value; 
                OnPropertyChanged();
            }
        }

        public void Init()
        {
            ShowProgress = true;
            _backgroundWorker.DoWork += LoadHistory;
            _backgroundWorker.RunWorkerCompleted += LoadingFinished;
            _backgroundWorker.RunWorkerAsync();
        }

        private void LoadingFinished(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
        {
            ShowProgress = false;
        }

        private void LoadHistory(object sender, DoWorkEventArgs e)
        {
            var history = _svnRepository.GetHistory(_connection).ToList();
            CommitList = new ObservableCollection<SvnCommitModel>(history);            
        }
    }
}