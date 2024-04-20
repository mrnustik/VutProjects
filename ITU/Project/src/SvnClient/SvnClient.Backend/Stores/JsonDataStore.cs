using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace SvnClient.Backend.Stores
{
    public abstract class JsonDataStore<T>
    {
        public abstract string StoreFilePath { get; }

        public void Store(T data)
        {
            var list = GetAll().ToList();
            list.Add(data);
            StoreData(list);
        }

        public virtual void Update(T data)
        {
            var list = GetAll().ToList();
            list.Remove(data);
            list.Add(data);
            StoreData(list);
        }

        public IEnumerable<T> GetAll()
        {
            if (!File.Exists(StoreFilePath)) return Enumerable.Empty<T>();
            var text = File.ReadAllText(StoreFilePath);
            try
            {
                var list = JsonConvert.DeserializeObject<IEnumerable<T>>(text);
                return list;
            }
            catch (Exception)
            {
                return Enumerable.Empty<T>();
            }
        }

        public virtual void Delete(T data)
        {
            var list = GetAll().ToList();
            list.Remove(data);
            StoreData(list);
        }

        protected void StoreData(IList<T> list)
        {
            var fileInfo = new FileInfo(StoreFilePath);
            fileInfo.Directory.Create();
            var json = JsonConvert.SerializeObject(list);
            File.WriteAllText(StoreFilePath, json);
        }
    }
}
