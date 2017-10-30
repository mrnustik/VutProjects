using System.ComponentModel.DataAnnotations;

namespace InformationSystem.DAL.Entities
{
    public class CarEntity : BaseEntity
    {
        [Required]
        public string CarIdentification { get; set; }
        [Required]
        public virtual UserEntity Owner { get; set; }
    }
}