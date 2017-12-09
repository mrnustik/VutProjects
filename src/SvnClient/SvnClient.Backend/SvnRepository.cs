using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.RegularExpressions;
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
                        Changes = commit.ChangedPaths?
                        .Select(item => new SvnChangeModel
                        {
                            NodeType = SvnNodeKindToNodeType(item.NodeKind),
                            Path = item.Path,
                            Type = SvnActionToSvnChangeType(item.Action)
                        })
                    };
                }
            }
        }

        private SvnNodeType SvnNodeKindToNodeType(SvnNodeKind itemNodeKind)
        {
            switch (itemNodeKind)
            {
                case SvnNodeKind.Directory:
                    return SvnNodeType.Directory;
                case SvnNodeKind.File:
                default:
                    return SvnNodeType.File;
            }
        }

        public IEnumerable<SvnFileLine> GetFileDiff(SvnConnection connection, SvnCommitModel commit, SvnCommitModel fromCommit, SvnChangeModel change)
        {
            if (change.NodeType == SvnNodeType.Directory) return Enumerable.Empty<SvnFileLine>();
            using (var client = new SharpSvn.SvnClient())
            {
                var fileUri = new Uri($"{connection.RemoteUri.AbsoluteUri}{change.Path}");
                var toTarget = new SvnUriTarget(fileUri, commit.Revision);
                var fromTarget = new SvnUriTarget(fileUri, fromCommit.Revision);
                var stream = new MemoryStream();
                client.Diff(fromTarget, toTarget, new SvnDiffArgs() { UseGitFormat = true, IgnoreContentType = true }, stream);
                var text = Encoding.UTF8.GetString(stream.ToArray());
                var lines = text.Split('\n')
                    .ToList();
                lines
                    .RemoveAll(l => l.StartsWith("diff")
                    || l.StartsWith("Index")
                    || l.StartsWith("===")
                    || l.StartsWith("---")
                    || l.StartsWith("+++")
                    || l.StartsWith("deleted file mode")
                    || l.StartsWith("new file mode")
                    || l.StartsWith("\\ No"));
                lines.Remove(lines.Last());
                return ParseSvnFileLines(lines);
            }
        }

        private IEnumerable<SvnFileLine> ParseSvnFileLines(List<string> lines)
        {
            int index = 0;
            foreach (var line in lines)
            {
                var lineNumber = "";

                var content = line;
                SvnChangeFileOperation operation = SvnChangeFileOperation.None;
                if (Regex.IsMatch(content, @"^\@\@ -\d+,\d+ \+\d+,\d+ \@\@"))
                {
                    var number = content.Substring(4, content.IndexOf(',') - 4);
                    var newLineNumber = Convert.ToInt32(number);
                    if (index != 0)
                    {
                        yield return new SvnFileLine
                        {
                            LineNumber = "...",
                            Content = "",
                            Operation = operation
                        };
                    }
                    index = newLineNumber;
                    continue;
                }
                if (line.StartsWith("+"))
                {
                    operation = SvnChangeFileOperation.Added;
                    index++;
                    lineNumber = index.ToString();
                }
                else if (line.StartsWith("-"))
                {
                    operation = SvnChangeFileOperation.Deleted;
                    lineNumber = "";
                }
                else
                {
                    index++;
                    lineNumber = index.ToString();
                }
                content = content.Replace("\r", "");
                yield return new SvnFileLine
                {
                    LineNumber = lineNumber,
                    Content = content,
                    Operation = operation
                };
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
