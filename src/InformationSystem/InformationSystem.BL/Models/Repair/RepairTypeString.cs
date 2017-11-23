using System.Collections.Generic;
using System.Linq;
using InformationSystem.DAL.Entities;

namespace InformationSystem.BL.Models.Repair
{
    public class RepairTypeString
    {
        public string String { get; set; }
        public RepairType RepairType { get; set; }

        public static List<RepairTypeString> GetAll()
        {
            return new List<RepairTypeString>
            {
                new RepairTypeString
                {
                    RepairType = RepairType.Engine,
                    String = "Motor"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Transmission,
                    String = "Převodovka"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Electronics,
                    String = "Elektronika"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Lights,
                    String = "Světla"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Brakes,
                    String = "Brzdy"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Battery,
                    String = "Baterie"
                },
                new RepairTypeString
                {
                    RepairType = RepairType.Body,
                    String = "Karoserie"
                }
            };
        }

        public static RepairTypeString GetRepairTypeString(RepairType repairType)
        {
            return Enumerable.First(GetAll(), r => r.RepairType == repairType);
        }
    }
}