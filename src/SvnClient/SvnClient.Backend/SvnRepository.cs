using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using SharpSvn;
using SharpSvn.Delta;
using SvnClient.Backend.Models;

namespace SvnClient.Backend
{
    public class SvnRepository
    {
        public IEnumerable<SvnCommitModel> GetHistory(SvnConnection connection)
        {
            using (var client = new SharpSvn.SvnClient())
            {
                client.GetLog(connection.LocalPath, new SvnLogArgs(), out var logItems);
                foreach (var commit in logItems)
                {
                    yield return new SvnCommitModel
                    {
                        Author = commit.Author,
                        Message = commit.LogMessage,
                        Time = commit.Time,
                        Revision = commit.Revision,
                        Changes = commit.ChangedPaths?.Select(item => new SvnChangeModel
                        {
                            Path = item.Path,
                            Type = SvnActionToSvnChangeType(item.Action)
                        })
                    };
                }
            }
        }

        public string GetFileDiff(SvnConnection connection,SvnCommitModel commit,SvnCommitModel fromCommit, SvnChangeModel change)
        {
            using (var client = new SharpSvn.SvnClient())
            {
                var fileUri = new Uri($"{connection.RemoteUri.AbsoluteUri}{change.Path}");
                var toTarget = new SvnUriTarget(fileUri, commit.Revision);
                var fromTarget = new SvnUriTarget(fileUri, fromCommit.Revision);
                var stream = new MemoryStream();
                client.Diff(fromTarget, toTarget, new SvnDiffArgs(){UseGitFormat = true, IgnoreContentType = true}, stream);
                var text = Encoding.UTF8.GetString(stream.ToArray());
                return text;
            }
        }

        private SvnChangeType SvnActionToSvnChangeType(SvnChangeAction itemAction)
        {
            switch (itemAction)
            {
                case SvnChangeAction.Add:
                    return SvnChangeType.Add;
                case SvnChangeAction.Delete:
                    return SvnChangeType.Delete;
                case SvnChangeAction.Modify:
                    return SvnChangeType.Modify;
                case SvnChangeAction.Replace:
                    return SvnChangeType.Replace;
            }
            return SvnChangeType.Add;
        }
    }
}
