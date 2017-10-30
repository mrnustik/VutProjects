using System.ComponentModel.DataAnnotations;

namespace InformationSystem.DAL.Entities
{
    public class UsedMaterialEntity : BaseEntity
    {
        [Required]
        public virtual MaterialEntity Material { get; set; }
        [Required]
        public virtual RepairEntity Repair { get; set; }
        public double Ammount { get; set; } = 1.0d;
    }
}