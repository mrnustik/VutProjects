using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SvnClient.Backend
{
    public class SvnConnection
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public Uri RemoteUri { get; set; }
        public string LocalPath { get; set; }

        private SvnConnection() { }

        public static SvnConnection OpenConnection(string name, string remoteUri)
        {
            return OpenConnection(name, new Uri(remoteUri));
        }

        public static SvnConnection OpenConnection(string name, Uri remoteUri)
        {
            var localPath = Path.Combine(Path.GetTempPath(), "SvnClient", new Random().Next().ToString());
            using(var client = new SharpSvn.SvnClient())
            {

                if (client.CheckOut(new SharpSvn.SvnUriTarget(remoteUri), localPath))
                {
                    return new SvnConnection
                    {
                        Name = name,
                        RemoteUri = remoteUri,
                        LocalPath = localPath
                    };
                }
            }
            return null;
        }
    }
}
