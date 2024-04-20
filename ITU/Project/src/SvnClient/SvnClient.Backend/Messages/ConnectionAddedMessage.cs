namespace SvnClient.Backend.Messages
{
    public class ConnectionAddedMessage
    {
        public ConnectionAddedMessage(SvnConnection connection)
        {
            Connection = connection;
        }

        public SvnConnection Connection { get; }
    }
}