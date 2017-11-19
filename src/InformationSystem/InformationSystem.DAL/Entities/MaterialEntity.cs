using System;

namespace InformationSystem.DAL.Entities
{
    public class MaterialEntity : BaseEntity
    {
        public string Name { get; set; }
        public int Price { get; set; }
    }
}