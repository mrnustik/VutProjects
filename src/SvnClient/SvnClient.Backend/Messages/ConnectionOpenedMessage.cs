using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SvnClient.Backend.Messages
{
    public class ConnectionOpenedMessage
    {
        public SvnConnection Connection { get; }

        public ConnectionOpenedMessage(SvnConnection connection)
        {
            Connection = connection;
        }
    }

    public class ConnectionAddedMessage
    {
        public ConnectionAddedMessage(SvnConnection connection)
        {
            Connection = connection;
        }

        public SvnConnection Connection { get; }
    }
}
