using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace InformationSystem.DAL.Entities
{
    public class RepairEntity : BaseEntity
    {
        public string Description { get; set; }
        public bool Done { get; set; } = false;
        [Required]
        public RepairType RepairType { get; set; }
        [Required]
        public DateTime ReservationTime { get; set; }
        [Required]
        public int Duration { get; set; }
        [Required]
        public virtual UserEntity Mechanic { get; set; }
        [Required]
        public virtual CarEntity Car { get; set; }
        public IList<UsedMaterialEntity> UsedMaterials { get; set; } = new List<UsedMaterialEntity>();
    }
}