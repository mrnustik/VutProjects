namespace SvnClient.Backend.Models
{
    public class SvnChangeModel
    {
        public SvnChangeType Type { get; set; }
        public string Path { get; set; }
    }
}