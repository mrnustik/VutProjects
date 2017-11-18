using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL.Entities;

namespace InformationSystem.BL.Models.Repair
{
    public class RepairDetailModel
    {
        [Required]
        public CarListModel Car { get; set; }
        [Required]
        public RepairType RepairType { get; set; }
        [Required]
        public string Description { get; set; } = "Text";
        public DateTime ReservationDate { get; set; } = DateTime.Today;
        public bool Done { get; set; }
        public UserModel Mechanic { get; set; }
    }
}
