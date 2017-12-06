using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SvnClient.Backend.Models
{
    public class SvnCommitModel
    {
        public string Author { get; set; }
        public string Message { get; set; }
        public DateTime Time { get; set; }
        public long Revision { get; set; }
        public IEnumerable<SvnChangeModel> Changes { get; set; }
    }

    public class SvnChangeModel
    {
        public SvnChangeType Type { get; set; }
        public string Path { get; set; }
    }

    public enum SvnChangeType
    {
        Add,
        Delete,
        Modify,
        Replace
    }
}
