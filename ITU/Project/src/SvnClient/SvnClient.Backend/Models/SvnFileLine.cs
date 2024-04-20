namespace SvnClient.Backend.Models
{
    public class SvnFileLine
    {
        public string LineNumber { get; set; }
        public string Content { get; set; }
        public SvnChangeFileOperation Operation { get; set; }
    }
}