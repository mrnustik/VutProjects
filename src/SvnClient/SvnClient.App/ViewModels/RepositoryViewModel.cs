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
        private readonly BackgroundWorker _fileLoadingWorker = new BackgroundWorker { WorkerReportsProgress = true };
        private readonly CollectionViewSource _collectionViewSource = new CollectionViewSource();

        private SvnCommitModel _selectedCommit;
        private bool _showProgress;
        private ObservableCollection<SvnCommitModel> _commitList;
        private List<SvnFileLine> _fileDiff;
        private bool _showFileProgress;

        public RepositoryViewModel(SvnConnection connection, [NotNull] SvnRepository svnRepository)
        {
            _svnRepository = svnRepository ?? throw new ArgumentNullException(nameof(svnRepository));
            _connection = connection ?? throw new ArgumentNullException(nameof(connection));
            SearchCommand = new RelayCommand(Search);
            FileSelectedCommand = new RelayCommand(FileSelected);
        }

        public string Name => _connection.Name;

        public ICommand SearchCommand { get; }
        public ICommand FileSelectedCommand { get; }

        public bool ShowProgress
        {
            get => _showProgress;
            set
            {
                if (_showProgress == value) return;
                _showProgress = value; 
                OnPropertyChanged();
            }
        }

        public bool ShowFileProgress
        {
            get => _showFileProgress;
            set
            {
                if (_showFileProgress == value) return;
                _showFileProgress = value;
                OnPropertyChanged();
            }
        }


        public SvnCommitModel SelectedCommit
        {
            get => _selectedCommit;
            set
            {
                if (_selectedCommit == value) return;
                _selectedCommit = value; 
                OnPropertyChanged();
            }
        }
        
        public ObservableCollection<SvnCommitModel> CommitList
        {
            get => _commitList;
            set {
                if (_commitList == value) return;
                _commitList = value; 
                OnPropertyChanged();
            }
        }

        public ICollectionView CollectionView => _collectionViewSource.View;

        public List<SvnFileLine> FileDiff
        {
            get => _fileDiff;
            set
            {
                if (_fileDiff == value) return;
                _fileDiff = value;
                OnPropertyChanged();
            }
        }


        public void Init()
        {
            ShowProgress = true;
            _backgroundWorker.DoWork += LoadHistory;
            _backgroundWorker.RunWorkerCompleted += LoadingFinished;
            _backgroundWorker.RunWorkerAsync();
            _fileLoadingWorker.DoWork += LoadFileDiff;
            _fileLoadingWorker.RunWorkerCompleted += FileDiffLoaded;
        }

        private void FileSelected(object o)
        {
            if (o is SvnChangeModel file && SelectedCommit.Revision != 0)
            {
                ShowFileProgress = true;
                if (_fileLoadingWorker.IsBusy)
                {
                    _fileLoadingWorker.CancelAsync();
                }
                _fileLoadingWorker.RunWorkerAsync(file);
            }
        }

        private void FileDiffLoaded(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
        {
            ShowFileProgress = false;
            OnPropertyChanged(nameof(FileDiff));
        }

        private void LoadFileDiff(object sender, DoWorkEventArgs doWorkEventArgs)
        {
            if (doWorkEventArgs.Argument is SvnChangeModel file)
            {
                _fileDiff = _svnRepository.GetFileDiff(_connection, SelectedCommit, CommitList.ElementAt(CommitList.IndexOf(SelectedCommit) + 1), file)
                    .ToList();
            }
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
                    if (obj is SvnCommitModel commit)
                    {
                        return (commit.Author != null && commit.Author.Contains(pattern))
                               || (commit.Message != null && commit.Message.Contains(pattern))
                               || (commit.Changes != null &&
                                   commit.Changes.Any(c => c.Path != null && c.Path.Contains(pattern)));
                    }
                    return false;
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