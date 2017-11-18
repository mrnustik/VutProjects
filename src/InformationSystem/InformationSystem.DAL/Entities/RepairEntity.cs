using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Internal;

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
        public string MechanicUserName { get; set; }
        [Required]
        public virtual CarEntity Car { get; set; }
        public IList<UsedMaterialEntity> UsedMaterials { get; set; } = new List<UsedMaterialEntity>();
    }
}