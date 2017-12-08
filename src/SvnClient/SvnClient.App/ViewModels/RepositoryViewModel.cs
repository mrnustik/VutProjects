using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using SvnClient.App.Annotations;
using SvnClient.App.Command;
using SvnClient.Backend;
using SvnClient.Backend.Models;

namespace SvnClient.App.ViewModels
{
    public class RepositoryViewModel : ViewModelBase
    {
        private readonly SvnRepository _svnRepository;
        private readonly SvnConnection _connection;
        private readonly BackgroundWorker _backgroundWorker = new BackgroundWorker();
        private readonly CollectionViewSource _collectionViewSource = new CollectionViewSource();

        public RepositoryViewModel(SvnConnection connection, [NotNull] SvnRepository svnRepository)
        {
            _svnRepository = svnRepository ?? throw new ArgumentNullException(nameof(svnRepository));
            _connection = connection ?? throw new ArgumentNullException(nameof(connection));
            SearchCommand = new RelayCommand(Search);
        }

        private SvnCommitModel _selectedCommit;
        private bool _showProgress;
        private ObservableCollection<SvnCommitModel> _commitList;
        
        public string Name => _connection.Name;

        public ICommand SearchCommand { get; private set; }
        
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

        public ICollectionView CollectionView
        {
            get => _collectionViewSource.View;
        }


        public void Init()
        {
            ShowProgress = true;
            _backgroundWorker.DoWork += LoadHistory;
            _backgroundWorker.RunWorkerCompleted += LoadingFinished;
            _backgroundWorker.RunWorkerAsync();
        }

        private void Search(object o)
        {
            var pattern = (o as string);
            if (string.IsNullOrEmpty(pattern))
            {
                CollectionView.Filter = obj => true;
            }
            else
            {
                CollectionView.Filter = obj =>
                {
                    var commit = obj as SvnCommitModel;
                    return (commit.Author != null && commit.Author.Contains(pattern))
                                || (commit.Message != null && commit.Message.Contains(pattern))
                                || (commit.Changes != null && commit.Changes.Any(c => c.Path != null && c.Path.Contains(pattern)));
                };
            }
            OnPropertyChanged(nameof(CollectionView));
        }

        private void LoadingFinished(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
        {
            ShowProgress = false;
            _collectionViewSource.Source = CommitList;
            OnPropertyChanged(nameof(CollectionView));
        }

        private void LoadHistory(object sender, DoWorkEventArgs e)
        {
            var history = _svnRepository.GetHistory(_connection).ToList();
            CommitList = new ObservableCollection<SvnCommitModel>(history);
        }
    }
}