using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ServerApi.Models
{
    public class StoredValue
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public long Key { get; set; }
        public string Position { get; set; }
        public DateTime DateTime { get; set; }
        public double Value { get; set; }
    }

    public class Temperature : StoredValue { }

    public class Humidity : StoredValue { }
}