using System;
using System.ComponentModel.DataAnnotations;

namespace InformationSystem.DAL.Entities
{
    public class UsedMaterialEntity : BaseEntity
    {
        [Required]
        public Guid MaterialId { get; set; }
        public virtual MaterialEntity Material { get; set; }
        [Required]
        public Guid RepairId { get; set; }
        public virtual RepairEntity Repair { get; set; }
        public double Ammount { get; set; } = 1.0d;
    }
}