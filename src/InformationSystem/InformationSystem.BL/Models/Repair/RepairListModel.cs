using System;
using InformationSystem.DAL.Entities;

namespace InformationSystem.BL.Models.Repair
{
    public class RepairListModel
    {
        public Guid Id { get; set; }
        public RepairType RepairType { get; set; }
        public string CarName { get; set; }
        public string CarManufacturer { get; set; }
        public string CarType { get; set; }
        public DateTime ReservationDate { get; set; } = DateTime.Today;
        public string MechanicName { get; set; }
        public bool Done { get; set; }
        public string MechanicUserName { get; set; }
    }
}