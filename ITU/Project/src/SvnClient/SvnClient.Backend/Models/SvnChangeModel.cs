namespace SvnClient.Backend.Models
{
    public class SvnChangeModel
    {
        public SvnChangeType Type { get; set; }
        public SvnNodeType NodeType { get; set; }
        public string Path { get; set; }
    }

    public enum SvnNodeType
    {
        File,
        Directory
    }
}