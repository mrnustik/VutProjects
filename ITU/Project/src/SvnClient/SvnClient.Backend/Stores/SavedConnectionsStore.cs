using System;
using System.IO;
using System.Linq;

namespace SvnClient.Backend.Stores
{
    public class SavedConnectionsStore : JsonDataStore<SvnConnection>
    {
        public override string StoreFilePath =>
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "SvnClient/connections.json");

        public override void Update(SvnConnection data)
        {
            var connections = GetAll().ToList();
            connections.RemoveAll(f => f.Id == data.Id);
            connections.Add(data);
            StoreData(connections);
        }

        public override void Delete(SvnConnection data)
        {
            var connections = GetAll().ToList();
            connections.RemoveAll(f => f.Id == data.Id);
            StoreData(connections);
        }
    }
}
