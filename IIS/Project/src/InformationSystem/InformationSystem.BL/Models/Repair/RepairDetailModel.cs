using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Models.Material;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL.Entities;

namespace InformationSystem.BL.Models.Repair
{
    public class RepairDetailModel
    {
        public Guid Id { get; set; }
        public CarDetailModel Car { get; set; }
        [Required]
        public RepairType RepairType { get; set; }
        public string Description { get; set; }
        public int Duration { get; set; }
        public DateTime ReservationDate { get; set; } = DateTime.Today;
        public bool Done { get; set; }
        public UserModel Mechanic { get; set; }
        public IEnumerable<UsedMaterialModel> UsedMaterials { get; set; }
        public RepairTypeString TypeString => RepairTypeString.GetRepairTypeString(RepairType);
    }
}
