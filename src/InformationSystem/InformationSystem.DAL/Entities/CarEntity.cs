using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Identity;

namespace InformationSystem.DAL.Entities
{
    public class CarEntity : BaseEntity
    {
        [Required]
        public string CarIdentification { get; set; }
        [ForeignKey("OwnerId")]
        public virtual IdentityUser Owner { get; set; }
    }
}