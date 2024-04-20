using System;
using System.Collections.Generic;
using System.Text;

namespace InformationSystem.BL.Models.Material
{
    public class UsedMaterialModel
    {
        public Guid Id { get; set; }
        public MaterialModel Material { get; set; }
        public double Ammount { get; set; }
    }
}
