using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.DAL.Entities
{
    public class CarEntity : BaseEntity
    {
        [Required]
        public string Name { get; set; }
        [Required]
        public string CarIdentification { get; set; }
        public string CompanyName { get; set; }
        public string TypeName { get; set; }
        public string OwnerEmail { get; set; }
    }
}