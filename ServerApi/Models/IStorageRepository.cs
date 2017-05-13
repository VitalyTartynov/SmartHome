using System.Collections.Generic;

namespace ServerApi.Models
{
    public interface IStorageRepository<StoredValue>
    {
        void Add(StoredValue item);
        IEnumerable<StoredValue> GetAll();
        StoredValue Find(long key);
        void Remove(long key);
        void Update(StoredValue item);
    }
}