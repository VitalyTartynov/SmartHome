using System;
using System.Collections.Generic;
using System.Linq;

namespace ServerApi.Models
{
    public class TemperaturesRepository : IStorageRepository<Temperature>
    {
        private readonly StorageContext _context;

        public TemperaturesRepository(StorageContext context)
        {
            _context = context;

            if (!_context.Temperatures.Any())
                Add(new Temperature { Position = "Кухня", Value = 25.3, DateTime = DateTime.Now});
        }

        public void Add(Temperature item)
        {
            _context.Temperatures.Add(item);
            _context.SaveChanges();
        }

        public IEnumerable<Temperature> GetAll()
        {
            return _context.Temperatures.ToList();
        }

        public Temperature Find(long key)
        {
            return _context.Temperatures.FirstOrDefault(t => t.Key == key);
        }

        public void Remove(long key)
        {
            var entity = _context.Temperatures.First(t => t.Key == key);
            _context.Temperatures.Remove(entity);
            _context.SaveChanges();
        }

        public void Update(Temperature item)
        {
            _context.Temperatures.Update(item);
            _context.SaveChanges();
        }
    }
}