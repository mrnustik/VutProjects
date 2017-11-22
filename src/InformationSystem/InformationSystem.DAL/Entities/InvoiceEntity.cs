using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text;

namespace InformationSystem.DAL.Entities
{
    public class InvoiceEntity : BaseEntity
    {
        public int InvoiceNumber { get; set; }
        public string ClientAddress { get; set; }
        public DateTime Date { get; set; }
        public DateTime DueDate { get; set; }
        [Required]
        public Guid RepairId { get; set; }
        public virtual RepairEntity Repair { get; set; }
    }
}
